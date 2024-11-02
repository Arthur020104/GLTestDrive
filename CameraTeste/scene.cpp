// Bibliotecas OpenGL e GLM
#include "glad/glad.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

// Bibliotecas do projeto
#include "scene.h"
#include "light.h"
#include "gameObject.h"
#include "shader.h"

// Bibliotecas padrão C++
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

//TODO remover  Light** lightArr, const int& sizeLightArr
//Separar luz objs no constructor


Scene::Scene(GameObject** objArr, const int& sizeObjArr, Light** lightArr, const int& sizeLightArr, Camera* camera, const glm::mat4& projection)
{
   
   // return;
    projectionMat = projection;
    mainCamera = camera;

    if (sizeLightArr > MAX_LIGHTS) 
    {
        std::ostringstream oss;
        oss << "[ERROR] MAX LIGHT SIZE is " << MAX_LIGHTS << " and you are passing " << sizeLightArr << " lights";
        throw std::runtime_error(oss.str());
    }

    for (int i = 0; i < sizeLightArr; i++)
    {
        sceneLights[i] = lightArr[i];
        sceneLights[i]->lightsThatWereUpdated = &lightsChanged;
    }
    sizeOfLightsArr = sizeLightArr;
 
    sceneObjs.resize(sizeObjArr);
    for (int i = 0; i < sizeObjArr; i++)
    {
        sceneObjs[i] = objArr[i];
    }
   
    updateLightsCache();
}


void Scene::updateLightsCache()
{
    lightsPosWorldCache.resize(sizeOfLightsArr);
    lightsColorsCache.resize(sizeOfLightsArr);
    lightsIntensityCache.resize(sizeOfLightsArr);

    for (int i = 0; i < sizeOfLightsArr; i++) 
    {
        lightsPosWorldCache[i] = sceneLights[i]->getPos() * glm::mat3(sceneLights[i]->getLightModelMat());
        lightsColorsCache[i] = sceneLights[i]->getColor();
        lightsIntensityCache[i] = sceneLights[i]->getIntensity();
    }
  
}

void Scene::render()
{
    if (lightsChanged.size() > 0)//isso precisa ser otimizado atualizar apenas cache de luz que alterou 
    {
        updateLightsCache();
        lightsChanged.clear();
    }
    for (int i = 0; i < sceneObjs.size(); i++)
    {
        sceneObjs[i]->Update();
        sceneObjs[i]->AftherUpdate();
    }
    for (int i = 0; i < sceneObjs.size(); i++) 
    {
        if (!sceneObjs[i]->hasRenderAtribs())
        {
            continue; //Se objeto nao tiver atributos para renderizacao pular
        }
        

        sceneObjs[i]->prepareRender();

        glm::mat4 viewMatrix = mainCamera->getViewMatrix();
        if (i == 0 || sceneObjs[i - 1]->getShaderPointer() != sceneObjs[i]->getShaderPointer()) 
        {
            Shader* shaderProgram = sceneObjs[i]->getShaderPointer();
            shaderProgram->use();
            shaderProgram->setInt("numberOfLights", sizeOfLightsArr);
            shaderProgram->setFloatArray("lightIntensity", &lightsIntensityCache[0], lightsIntensityCache.size());
            shaderProgram->setArrayVec3("lightsColorValues", &lightsColorsCache[0], lightsColorsCache.size());
            shaderProgram->setArrayVec3("lightsPosWorld", &lightsPosWorldCache[0], lightsPosWorldCache.size());
            shaderProgram->setMat4("projection", projectionMat);
            shaderProgram->setMat4("view", viewMatrix);
            shaderProgram->setMat3("view3", glm::mat3(viewMatrix));
            
        }

        glDrawArrays(GL_TRIANGLES, 0, sceneObjs[i]->getVerticesNum() / 3);
        glBindVertexArray(0);
        sceneObjs[i]->disableTextures();
    }

    
}
void Scene::groupObjVectorByShader()
{
    std::set<Shader*> uniqueShaders;
    for (const auto& obj : sceneObjs) 
    {
        Shader* shader = obj->getShaderPointer();
        if (!shader) 
        {
            std::cout << "Shader is NULL\n";
        }
        uniqueShaders.insert(shader);
    }

    std::vector<GameObject*> newSceneObjects;
    newSceneObjects.reserve(sceneObjs.size());

    for (Shader* shader : uniqueShaders) 
    {
        for (const auto& obj : sceneObjs) 
        {
            if (obj->getShaderPointer() == shader) 
            {
                newSceneObjects.push_back(obj);
            }
        }
    }
    sceneObjs = std::move(newSceneObjects);
}