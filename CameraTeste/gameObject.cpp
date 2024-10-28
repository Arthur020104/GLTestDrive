#pragma once
#include <glad/glad.h>
#include "Libs/stb_image.h"
#include "transformController.h"
#include "shader.h"
#include <iostream>
#include "gameObject.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <functional>
#include "main.h"
#include <glm/glm/gtx/string_cast.hpp>
#include <set>

void generateTexture1(unsigned int& texture, const char* path)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Could not load texture: " << path << "\n";
        return;
    }
    std::cout << path << "\n";

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Adjust texture format based on the number of channels
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GameObject::GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const glm::vec4& color, const float* vertices, const int& numVertices, Shader* shaderProgramRef, const bool& isStatic) : TransformController(inicialPos, inicialRot)
{
    objectColor = color;

    shaderProgram = shaderProgramRef;
    isStaticObj = isStatic;
    

    
    verticesNum = numVertices;
    sizeOfVerticesObj = sizeof(vertices[0]) * verticesNum;


    verticesObj = new float[verticesNum]; 

    std::copy(vertices, vertices + verticesNum, verticesObj);

    // Resize vertex normals based on verticesNum
    vertexNormals.resize(3*verticesNum, 0.0f);
    
    updateNormals();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Could be sizeOfVertices + sizeOfVertices because is the same size but doing vertexNormals.size() * sizeof(float) to be explicit
    int sizeOfNormals = vertexNormals.size() * sizeof(float);

    glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesObj + sizeOfNormals, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesObj, verticesObj);


    glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesObj, sizeOfNormals, &vertexNormals[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeOfVerticesObj));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

GameObject::~GameObject()
{
    for (int textureId : texturesIds)
    {
        glDeleteTextures(1, (GLuint *)&textureId);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete[] verticesObj;
}
void GameObject::setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int sizeOfNormals = vertexNormals.size() * sizeof(float);

    glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesObj + sizeOfNormals + sizeOftextureCoords, nullptr, isStaticObj ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesObj, verticesObj);

    glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesObj, sizeOfNormals, &vertexNormals[0]);

    glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesObj + sizeOfNormals, sizeOftextureCoords, textureCoords);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeOfVerticesObj));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeOfVerticesObj + sizeOfNormals));
    glEnableVertexAttribArray(2);
   
   

    

    texturesIds.reserve(numberOfTextures);

    

    // Vetor para testes de blending
    std::vector<float> blend;
    blend.reserve(numberOfTextures);

    std::vector<int> textureUnits;
    textureUnits.reserve(numberOfTextures);

    for (int i = 0; i < numberOfTextures; i++)
    {
        unsigned int texturesId;
        generateTexture1(texturesId, paths[i]);
     
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texturesId);


        texturesIds.push_back(texturesId);
        textureUnits.push_back(i);

        // Valores de blend de teste
        blend.push_back(0.5f);
    }
    shaderProgram->use();
    shaderProgram->setIntArray("arrayOfTextures", &textureUnits[0], textureUnits.size());
    shaderProgram->setFloatArray("arrayOfBledingValues", &blend[0], blend.size());
    shaderProgram->setInt("numberOfTextures", numberOfTextures);

   
    glBindVertexArray(0);
}
void GameObject::prepareRender(glm::mat4& view, const glm::mat4& projection)
{
    shaderProgram->use();

    this->shaderProgram->setMat4("projection", projection);
    this->shaderProgram->setMat4("view", view);

    glm::mat4 model = this->getModelMatrix();
    this->shaderProgram->setMat4("model", model);

    this->shaderProgram->setMat3("model3", glm::mat3(model));
    this->shaderProgram->setMat3("view3", glm::mat3(view));

    shaderProgram->setVec4("color", objectColor);

    this->applyLighting();

    for (int i = 0; i < texturesIds.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texturesIds[i]);
    }
    glBindVertexArray(VAO);
}
void GameObject::setUpdateFunc(std::function<void(GameObject*)> func)
{
    updateFunc = func;
}
void GameObject::objUpdate()
{
    if (updateFunc != nullptr)
    {
        updateFunc(this);
    }
    
}
bool containsNaN(const glm::vec3& vec) 
{
    return std::isnan(vec.x) || std::isnan(vec.y) || std::isnan(vec.z);
}/*
void GameObject::recalculateWVectors()
{
    wValues.clear();
    wValues.reserve(sizeOfLights);
    wValues.resize(sizeOfLights);
    
    glm::vec3 pos = this->getPos();

    for (int i = 0; i < sizeOfLights; i++)
    {
        wValues[i] = (glm::length(lightsArr[i].getPos()) > 0 || containsNaN(lightsArr[i].getPos()) )? lightsArr[i].getPos() : -pos;
        //std::cout << "global" << "\n";
       // printVec3(wValues[i]);
        //std::cout << "local" << "\n";
        //printVec3(lightsArr[i].getW(getPos()));
    }
}*/
void GameObject::updateLighting(Light* l, const int& sizeOfLightArr)//should be done by the scene and just kepp the w vector
{
    // Store the light array and size
    lightsArr = l;
    sizeOfLights = sizeOfLightArr;

    lightColors.clear();
    lightColors.reserve(sizeOfLights);

    lightPos.clear();
    lightPos.reserve(sizeOfLights);

    lightIntensity.clear();
    lightIntensity.reserve(sizeOfLights);

    lightModelMatrix.clear();
    lightModelMatrix.reserve(sizeOfLights);

    lightColors.resize(sizeOfLights);
    lightPos.resize(sizeOfLights);
    lightIntensity.resize(sizeOfLights);
    lightModelMatrix.resize(sizeOfLights);

    glm::vec3 pos = this->getPos();

    for (int i = 0; i < sizeOfLights; i++)
    {
        lightPos[i] = (glm::length(lightsArr[i].getPos()) > 0 || containsNaN(lightsArr[i].getPos())) ? lightsArr[i].getPos() : -pos;
        lightColors[i] = lightsArr[i].getColor();
        lightIntensity[i] = lightsArr[i].getIntensity();
        lightModelMatrix[i] = glm::mat3(lightsArr[i].getModelMatrix());
    }
}



void GameObject::applyLighting()
{
    shaderProgram->use();
    /*This will be changed will be values of the scene not gameobject*/
    shaderProgram->setInt("numberOfLights", sizeOfLights); //deve ser passado apenas uma vez para shader 
    

    shaderProgram->setFloatArray("lightIntensity", &lightIntensity[0], lightIntensity.size()); //deve ser passado apenas uma vez para shader

    shaderProgram->setArrayVec3("lightsColorValues", &lightColors[0], lightColors.size()); //deve ser passado apenas uma vez para shader


    shaderProgram->setArrayMat3("lightModelMat", &lightModelMatrix[0], lightModelMatrix.size());
    /*This will be changed will be values of the scene not gameobject*/
    
    
    shaderProgram->setArrayVec3("lightsPos", &lightPos[0], lightPos.size());//deve ser passado para cada objeto no update do shader


}
struct Vec3Compare {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

void GameObject::updateNormals()
{
    std::fill(vertexNormals.begin(), vertexNormals.end(), 0.0f); // Reset normals

    // Interior point for the object (assuming the object is centered at the origin)
    glm::vec3 interiorPoint = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < verticesNum; i += 9)
    {
        glm::vec3 v1 = glm::vec3(verticesObj[i], verticesObj[i + 1], verticesObj[i + 2]);
        glm::vec3 v2 = glm::vec3(verticesObj[i + 3], verticesObj[i + 4], verticesObj[i + 5]);
        glm::vec3 v3 = glm::vec3(verticesObj[i + 6], verticesObj[i + 7], verticesObj[i + 8]);

        glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);

        if (containsNaN(normal) || glm::length(normal) < 1e-6f)
        {
            normal = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        else
        {
            normal = glm::normalize(normal);

            glm::vec3 vectorToFace = v1 - interiorPoint;  
            if (glm::dot(vectorToFace, normal) < 0.0f)
            {
                normal = -normal;
            }
        }

        vertexNormals[i] = normal.x;
        vertexNormals[i + 1] = normal.y;
        vertexNormals[i + 2] = normal.z;

        vertexNormals[i + 3] = normal.x;
        vertexNormals[i + 4] = normal.y;
        vertexNormals[i + 5] = normal.z;

        vertexNormals[i + 6] = normal.x;
        vertexNormals[i + 7] = normal.y;
        vertexNormals[i + 8] = normal.z;
    }


    for (int i = 0; i < vertexNormals.size(); i += 3)
    {
        glm::vec3 accumulatedNormal = glm::vec3(vertexNormals[i], vertexNormals[i + 1], vertexNormals[i + 2]);
        glm::vec3 normalizedNormal = glm::normalize(accumulatedNormal);

        vertexNormals[i] = normalizedNormal.x;
        vertexNormals[i + 1] = normalizedNormal.y;
        vertexNormals[i + 2] = normalizedNormal.z;
    }


    // Atualizar dados do buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesObj, vertexNormals.size() * sizeof(float), &vertexNormals[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}