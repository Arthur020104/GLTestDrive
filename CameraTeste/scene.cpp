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
    lightsPosCache.resize(sizeOfLightsArr);
    lightsColorsCache.resize(sizeOfLightsArr);
    lightsIntensityCache.resize(sizeOfLightsArr);
    lightsModelMatrixCache.resize(sizeOfLightsArr);

    for (int i = 0; i < sizeOfLightsArr; i++) 
    {
        lightsPosCache[i] = sceneLights[i]->getPos();
        lightsColorsCache[i] = sceneLights[i]->getColor();
        lightsIntensityCache[i] = sceneLights[i]->getIntensity();
        lightsModelMatrixCache[i] = sceneLights[i]->getModelMatrix();
    }
  
}

void Scene::render()
{
    for (int i = 0; i < sceneObjs.size(); i++) 
    {
        sceneObjs[i]->objUpdate();
        sceneObjs[i]->prepareRender();

        glm::mat4 viewMatrix = mainCamera->getViewMatrix();
        if (i == 0 || sceneObjs[i - 1]->getShaderPointer() != sceneObjs[i]->getShaderPointer()) 
        {
            Shader* shaderProgram = sceneObjs[i]->getShaderPointer();
            if (shaderProgram) 
            {
                shaderProgram->use();
                shaderProgram->setInt("numberOfLights", sizeOfLightsArr);
                shaderProgram->setFloatArray("lightIntensity", &lightsIntensityCache[0], lightsIntensityCache.size());
                shaderProgram->setArrayVec3("lightsColorValues", &lightsColorsCache[0], lightsColorsCache.size());
                shaderProgram->setArrayMat3("lightModelMat", &lightsModelMatrixCache[0], lightsModelMatrixCache.size());
                shaderProgram->setArrayVec3("lightsPos", &lightsPosCache[0], lightsPosCache.size());
                shaderProgram->setMat4("projection", projectionMat);
                shaderProgram->setMat4("view", viewMatrix);
                shaderProgram->setMat3("view3", glm::mat3(viewMatrix));
            }
        }

        glDrawArrays(GL_TRIANGLES, 0, sceneObjs[i]->getVerticesNum() / 3);
    }
    glBindVertexArray(0); 
}
