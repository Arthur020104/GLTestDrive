#pragma once

// Biblioteca GLM
#include <glm/glm/gtc/matrix_transform.hpp>
#include <set>
// Biblioteca do projeto
#include "material.h"
#include "transformController.h"
#include "gameObject.h"


class Light : public GameObject
{
public:
    // Construtores
    Light(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const glm::vec3& color, bool isDirectional, float intensity)
        : GameObject(inicialPos, inicialRot),color(color),isDirectional(isDirectional),intensity(intensity),lightTransform(TransformController(glm::vec3(0.0f), glm::vec3(0.0f))) {}


    Light()
        : GameObject(glm::vec3(0.0f), glm::vec3(0.0f)),color(0.0f),isDirectional(false),intensity(1.0f),lightTransform(TransformController(glm::vec3(0.0f), glm::vec3(0.0f))) {}


    // Métodos públicos
    glm::vec3 getW(const glm::vec3& obj);
    glm::vec3 getColor();
    float getIntensity();

    // Operador de atribuição
    Light& operator=(const Light& other)
    {
        if (this != &other)
        {
            GameObject::operator=(other);
            color = other.color;
            isDirectional = other.isDirectional;
            intensity = other.intensity;
        }
        return *this;
    }

    void setPos(const glm::vec3& newPos) override;
    void setRot(const glm::vec3& newRot) override;
    void setColor(const glm::vec3& newColor);
    void setModelMatixMode(const unsigned short& newModelMatrixMode) override;
  //  void setScale(const glm::vec3& newScale) override;
    void prepareRender() override;

    glm::mat4 getLightModelMat();
    std::set<Light*>* lightsThatWereUpdated = nullptr;//This is going to be a pointer to the set in the scene class

    TransformController lightTransform;

private:
    // Atributos privados
    glm::vec3 color;
    bool isDirectional;
    float intensity;

    
   
 
};
