#pragma once

// Biblioteca GLM
#include <glm/glm/gtc/matrix_transform.hpp>

// Biblioteca do projeto
#include "transformController.h"

class Light : public TransformController
{
public:
    // Construtores
    Light(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const glm::vec3& color, bool isDirectional, float intensity)
        : TransformController(inicialPos, inicialRot), color(color), isDirectional(isDirectional), intensity(intensity) {}

    Light()
        : TransformController(glm::vec3(0.0f), glm::vec3(0.0f)), color(0.0f), isDirectional(false), intensity(1.0f) {}

    // Métodos públicos
    glm::vec3 getW(const glm::vec3& obj);
    glm::vec3 getColor();
    float getIntensity();

    // Operador de atribuição
    Light& operator=(const Light& other)
    {
        if (this != &other)
        {
            TransformController::operator=(other);
            color = other.color;
            isDirectional = other.isDirectional;
            intensity = other.intensity;
        }
        return *this;
    }

private:
    // Atributos privados
    glm::vec3 color;
    bool isDirectional;
    float intensity;
};
