#pragma once
#include <glad/glad.h>
#include "Libs/stb_image.h"
#include "shader.h"
#include <iostream>
#include "gameObject.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <functional>
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


GameObject::GameObject(glm::vec3 inicialPos, glm::vec3 inicialRot, const float* vertices, const int& sizeOfVertices, Shader* shaderProgramRef)
{
	objPos = inicialPos;
	objRot = inicialRot;
    shaderProgram = shaderProgramRef;
    verticesNum = sizeOfVertices / sizeof(float);

    sizeOfVerticesObj = sizeOfVertices;
    verticesObj = new float[sizeOfVertices];
    std::copy(vertices, vertices + sizeOfVertices, verticesObj);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}
GameObject::~GameObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete[] verticesObj;
}
void GameObject::setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

   
    glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesObj + sizeOftextureCoords, nullptr, GL_STATIC_DRAW);


    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesObj, verticesObj);


    glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesObj, sizeOftextureCoords, textureCoords);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeOfVerticesObj));
    glEnableVertexAttribArray(1);

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
void GameObject::prepareRender()
{
    shaderProgram->use();
    for (int i = 0; i < texturesIds.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texturesIds[i]);
    }
    glBindVertexArray(VAO);
    
}
glm::mat4 GameObject::getModelMatrix()
{
    glm::mat4 objPosMat = glm::translate(glm::mat4(1.0f), objPos);

    glm::mat4 objRotMat = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(objRot.z)), glm::vec3(0.0f, 0.0f, 1.0f));//rotate around x
    objRotMat = glm::rotate(objRotMat, glm::radians(static_cast<float>(objRot.y)), glm::vec3(0.0f, 1.0f, 0.0f));//rotate around y
    objRotMat = glm::rotate(objRotMat, glm::radians(static_cast<float>(objRot.x)), glm::vec3(1.0f, 0.0f, 0.0f));//rotate around z

    glm::mat4 objScaleMat = glm::scale(glm::mat4(1.0f), objScale);
    
    return  objPosMat * objRotMat * objScaleMat;
}
glm::vec3 GameObject::getPos()
{
    return objPos;
}
glm::vec3 GameObject::getRot()
{
    return objRot;
}
glm::vec3 GameObject::getScale()
{
    return objScale;
}

void GameObject::setPos(const glm::vec3& newPos)
{
    objPos = newPos;
}
void GameObject::setRot(const glm::vec3& newRot)
{
    objRot = newRot;
}
void GameObject::setScale(const glm::vec3& newScale)
{
    objScale = newScale;
}

glm::vec3 GameObject::forward()
{
    return glm::vec3(
        glm::cos(objRot.x) * glm::sin(objRot.y),  
        glm::sin(objRot.x),                       
        glm::cos(objRot.x) * glm::cos(objRot.y) 
    );
}
void GameObject::setUpdateFunc(std::function<void(GameObject*)> func)
{
    updateFunc = func;
}
void GameObject::objUpdate()
{
    updateFunc(this);
}

