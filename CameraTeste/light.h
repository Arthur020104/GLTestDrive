#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include "transformController.h"

class Light : public TransformController
{
public:
    Light(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const glm::vec3& color, bool isDirectional, float intensity): TransformController(inicialPos, inicialRot), color(color), isDirectional(isDirectional), intensity(intensity) {}

    glm::vec3 getW(const glm::vec3& obj);
    glm::vec3 getColor();
    float getIntensity();

private:
    glm::vec3 color;
    bool isDirectional; 
    float intensity;
};
