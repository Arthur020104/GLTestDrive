#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
// Implementação da biblioteca STB para carregar imagens
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "shader.h"

// Valores padrão para as propriedades do Material
const float DEFAULT_ROUGHNESS = 500.0f;
const int DEFAULT_SPECULAR_AMOUNT = 1;
const glm::vec4 DEFAULT_COLOR = glm::vec4(0.3f, 0.3f, 0.5f, 1.0f);
const glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.1f, 0.1f, 0.1f);
const int DEFAULT_DIFFUSE = 0;
const glm::vec3 DEFAULT_SPECULAR_VEC = glm::vec3(DEFAULT_SPECULAR_AMOUNT);



class Material
{
public:
    glm::vec3 ambient;
    
    float roughness;
    
    glm::vec4 color;

    // Construtor principal
    Material(const glm::vec4& colorV = DEFAULT_COLOR, const float& roughnessAmt = DEFAULT_ROUGHNESS, const glm::vec3& ambientV = DEFAULT_AMBIENT) :
        color(colorV), roughness(roughnessAmt), specular(DEFAULT_SPECULAR_AMOUNT), ambient(ambientV), diffuse(DEFAULT_DIFFUSE) {}



    void LoadMaterialDataToShader(Shader* s);

    void LoadDiffuseMap(const char* path);
    void LoadSpecular(const char* path);
    //createDisable texture ;
    void UnbindMaterial();
private:
    unsigned int diffuseMap;
    int diffuse;
    unsigned int specularMap;
    int specular;
    unsigned int generateTexture(const char* path);
    
};
//void generateTexture(unsigned int& texture, const char* path);