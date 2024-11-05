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


Scene::Scene(GameObject** objArr, const int& sizeObjArr, Camera* camera, const glm::mat4& projection)
{
   
   // return;
    projectionMat = projection;
    mainCamera = camera;

 
    sceneObjs.resize(sizeObjArr);
    for (int i = 0; i < sizeObjArr; i++)
    {
        sceneObjs[i] = objArr[i];
        Light* p = dynamic_cast<Light*>(objArr[i]);
        if (p)
            sceneLights.push_back(p);
    }
   
    if (sceneLights.size() > MAX_LIGHTS)
    {
        std::ostringstream oss;
        oss << "[ERROR] MAX LIGHT SIZE is " << MAX_LIGHTS << " and you are passing " << sceneLights.size() << " lights";
        throw std::runtime_error(oss.str());
    }

    for (size_t i = 0; i < sceneLights.size(); i++)
    {
        sceneLights[i]->lightsThatWereUpdated = &lightsChanged;
    }

    updateLightsCache();
}


void Scene::updateLightsCache()
{
    size_t sizeOfLightsVec = sceneLights.size();
    lightsPosWorldCache.resize(sizeOfLightsVec);
    lightsColorsCache.resize(sizeOfLightsVec);
    lightsIntensityCache.resize(sizeOfLightsVec);

    for (int i = 0; i < sizeOfLightsVec; i++) 
    {
        lightsPosWorldCache[i] = sceneLights[i]->getPos() * glm::mat3(sceneLights[i]->getLightModelMat());
        lightsColorsCache[i] = sceneLights[i]->getColor();
        lightsIntensityCache[i] = sceneLights[i]->getIntensity();
    }
  
}

void Scene::updateLightsChangedCache()
{
    for (auto element : lightsChanged)
    {
        auto it = std::find(sceneLights.begin(), sceneLights.end(), element);
        size_t index = std::distance(sceneLights.begin(), it);
        lightsPosWorldCache[index] = element->getPos() * glm::mat3(element->getLightModelMat());
        lightsColorsCache[index] = element->getColor();
        lightsIntensityCache[index] = element->getIntensity();
    }
    lightsChanged.clear();
}

void Scene::render()
{
    if (lightsChanged.size() > 0)
        updateLightsChangedCache();


    for (int i = 0; i < sceneObjs.size(); i++)
    {
        sceneObjs[i]->BeforeUpdate();
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
            shaderProgram->setInt("numberOfLights", sceneLights.size());
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