#pragma once
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "transformController.h"
#include "main.h"
#include <iostream>
#include <stdexcept>s


void TransformController::updateModelMatrix()
{
    glm::mat4 objPosMat = glm::translate(glm::mat4(1.0f), objPos);

    glm::mat4 objRotMat = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(objRot.z)), glm::vec3(0.0f, 0.0f, 1.0f));//rotate around z
    objRotMat = glm::rotate(objRotMat, glm::radians(static_cast<float>(objRot.y)), glm::vec3(0.0f, 1.0f, 0.0f));//rotate around y
    objRotMat = glm::rotate(objRotMat, glm::radians(static_cast<float>(objRot.x)), glm::vec3(1.0f, 0.0f, 0.0f));//rotate around x

    glm::mat4 objScaleMat = glm::scale(glm::mat4(1.0f), objScale);

    switch (modelMatrixMode)
    {
        case 0:
            ModelMatrix = objPosMat * objRotMat * objScaleMat;
            break;
        case 1:
            ModelMatrix = objRotMat * objPosMat * objScaleMat;
            break;
        case 2: 
            ModelMatrix = objScaleMat * objPosMat * objRotMat;
            break;
        default://Maybe add the other combinations
            ModelMatrix = objPosMat * objRotMat * objScaleMat;
            break;
    }
    
}
void TransformController::setModelMatixMode(const unsigned short& newModelMatrixMode)
{
    if (newModelMatrixMode > 2)
    {
        throw std::runtime_error("[ERROR] Model matrix only have 3 modes [0-2], you passed: " + newModelMatrixMode);
    }

    modelMatrixMode = newModelMatrixMode;
    updateModelMatrix();
}
glm::mat4 TransformController::getModelMatrix()
{
    return ModelMatrix;
}

glm::vec3 TransformController::getPos()
{
    return objPos;
}
glm::vec3 TransformController::getRot()
{
    return objRot;
}
glm::vec3 TransformController::getScale()
{
    return objScale;
}
bool containsNaN2(const glm::vec3& vec)
{
    return std::isnan(vec.x) || std::isnan(vec.y) || std::isnan(vec.z);
}
void TransformController::setPos(const glm::vec3& newPos)
{
    objPos = newPos;
    updateModelMatrix();
}
void TransformController::setRot(const glm::vec3& newRot)
{
    objRot = newRot;
    updateModelMatrix();
}
void TransformController::setScale(const glm::vec3& newScale)
{
    objScale = newScale;
    updateModelMatrix();
}

glm::vec3 TransformController::forward()
{
    return glm::vec3(
        glm::cos(objRot.x) * glm::sin(objRot.y),
        glm::sin(objRot.x),
        glm::cos(objRot.x) * glm::cos(objRot.y)
    );
}
glm::vec3 TransformController::right()
{
    return glm::vec3(
        glm::cos(objRot.y),
        0.0f,
        glm::sin(objRot.y)
    );
}
