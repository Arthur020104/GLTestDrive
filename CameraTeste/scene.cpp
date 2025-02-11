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
    lightsCache.resize(sizeOfLightsVec);

    for (int i = 0; i < sizeOfLightsVec; i++) 
    {
        updateElementCache(sceneLights[i]);
    }
  
}
void Scene :: updateElementCache(Light* element)
{
    auto it = std::find(sceneLights.begin(), sceneLights.end(), element);
    size_t index = std::distance(sceneLights.begin(), it);

    lightsCache[index].posWorldCache = element->getPos() * glm::mat3(element->getLightModelMat());
    lightsCache[index].colorsCache = element->getColor();
    lightsCache[index].intensityCache = element->getIntensity();
    lightsCache[index].ambientCache = element->getAmbient();
    lightsCache[index].diffuseCache = element->getDiffuse();
    lightsCache[index].specularCache = element->getSpecular();
    lightsCache[index].directionalCache = element->getIsDirectional();

    lightsCache[index].directionCache = element->getDirection();
    lightsCache[index].cutOffCache = element->getCutOff();

    lightsCache[index].outerCutOffCache = element->getOuterCutOff();
}
void Scene::updateLightsChangedCache()
{
    for (auto element : lightsChanged)
    {
        updateElementCache(element);
    }
    lightsChanged.clear();
}

void Scene::render()
{
    for (int i = 0; i < sceneObjs.size(); i++)
    {
        sceneObjs[i]->BeforeUpdate();
        sceneObjs[i]->Update();
        sceneObjs[i]->AftherUpdate();
    }
    if (lightsChanged.size() > 0)
        updateLightsChangedCache();
    for (int i = 0; i < sceneObjs.size(); i++) 
    {
        if (!sceneObjs[i]->hasRenderAtribs())
        {
            continue; //Se objeto nao tiver atributos para renderizacao pular
        }
        

        sceneObjs[i]->prepareRender();

        glm::mat4 viewMatrix = mainCamera->getViewMatrix();
        glm::vec3 cameraPos = mainCamera->getPos();
        //this->updateLightsChangedCache();
        if (i == 0 || sceneObjs[i - 1]->getShaderPointer() != sceneObjs[i]->getShaderPointer()) //instead of checking if the last object had the same shader, check if lights were set to any of the shaders this loop
        {
            Shader* shaderProgram = sceneObjs[i]->getShaderPointer();
            shaderProgram->use();
            shaderProgram->setInt("numberOfLights", sceneLights.size());

            for (int i = 0; i < sceneLights.size(); ++i) 
            {
                std::string baseName = "lights[" + std::to_string(i) + "]";
                shaderProgram->setVec3(baseName + ".positionWorld", lightsCache[i].posWorldCache);
                shaderProgram->setVec3(baseName + ".colorValue", lightsCache[i].colorsCache);
                shaderProgram->setFloat(baseName + ".intensity", lightsCache[i].intensityCache);

                shaderProgram->setVec3(baseName + ".ambient", lightsCache[i].ambientCache);
                shaderProgram->setVec3(baseName + ".diffuse", lightsCache[i].diffuseCache);
                shaderProgram->setVec3(baseName + ".specular", lightsCache[i].specularCache);

                shaderProgram->setBool(baseName + ".isDirectional", lightsCache[i].directionalCache);

                shaderProgram->setFloat (baseName + ".cutOff", lightsCache[i].cutOffCache);
                shaderProgram->setVec3(baseName + ".direction", lightsCache[i].directionCache);

                shaderProgram->setFloat(baseName + ".outerCutOff", lightsCache[i].outerCutOffCache);
            }
            shaderProgram->setVec3("cameraPos", cameraPos);
            shaderProgram->setMat4("projection", projectionMat);
            shaderProgram->setMat4("view", viewMatrix);
            shaderProgram->setMat3("view3", glm::mat3(viewMatrix));
            
        }

        glDrawArrays(GL_TRIANGLES, 0, sceneObjs[i]->getVerticesNum() / 3);
        
        sceneObjs[i]->Unbind();
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