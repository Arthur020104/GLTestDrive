#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>

// Valores padrão para as propriedades do Material
const float DEFAULT_ROUGHNESS = 500.0f;
const float DEFAULT_SPECULAR_AMOUNT = 0.5f;
const glm::vec4 DEFAULT_COLOR = glm::vec4(0.3f, 0.3f, 0.5f, 1.0f);
const glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.1f, 0.1f, 0.1f);
const glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.7f, 0.7f, 0.7f);
const glm::vec3 DEFAULT_SPECULAR_VEC = glm::vec3(DEFAULT_SPECULAR_AMOUNT);

class Material
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    float roughness;
    glm::vec3 amountOfSpecular;
    glm::vec4 color;

    // Construtor principal
    Material(const glm::vec4& colorV = DEFAULT_COLOR, const float& roughnessAmt = DEFAULT_ROUGHNESS, const glm::vec3& specularAmt = DEFAULT_SPECULAR_VEC, const glm::vec3& ambientV = DEFAULT_AMBIENT, const glm::vec3& diffuseV = DEFAULT_DIFFUSE): color(colorV), roughness(roughnessAmt), amountOfSpecular(specularAmt), ambient(ambientV), diffuse(diffuseV) {}

   
    Material(const glm::vec4& colorV, const float& roughnessAmt, const float& specularAmt, const glm::vec3& ambientV = DEFAULT_AMBIENT, const glm::vec3& diffuseV = DEFAULT_DIFFUSE): color(colorV), roughness(roughnessAmt), amountOfSpecular(glm::vec3(specularAmt)), ambient(ambientV), diffuse(diffuseV) {}

};
