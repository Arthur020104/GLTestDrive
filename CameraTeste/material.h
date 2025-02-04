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
const glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.3f, 0.3f, 0.3f);
const int DEFAULT_DIFFUSE = 0;
const int DEFAULT_EMISSION = 2;
const glm::vec3 DEFAULT_SPECULAR_VEC = glm::vec3(DEFAULT_SPECULAR_AMOUNT);



class Material
{
public:

    // Construtor principal
    Material(const glm::vec4& colorV = DEFAULT_COLOR, const float& brightnessAmt = DEFAULT_ROUGHNESS, const glm::vec3& ambientV = DEFAULT_AMBIENT) :
        color(colorV), brightness(brightnessAmt), specular(DEFAULT_SPECULAR_AMOUNT), ambient(ambientV), diffuse(DEFAULT_DIFFUSE), emission(DEFAULT_EMISSION) {}



    void LoadMaterialDataToShader(Shader* s);
    void LoadMaterialDataToShader(Shader* s, glm::vec3 scale);

    void LoadDiffuseMap(const char* path);
    void LoadSpecularMap(const char* path);
    void LoadEmissionMap(const char* path);


    void setRepeatTexture(const bool& repeat) { this->repeatTexture = repeat; };
    void setRepeatTextureFactor(const glm::vec3& repeatFactor) { this->repeatTextureFactor = repeatFactor; };
    void setSpecularMulti(const float& multi) { this->specularMulti = multi; };
    //createDisable texture ;
    void UnbindMaterial();
private:
    float diffuseMulti = 1.0f;
    float specularMulti = 1.0f;
    float emMulti = 1.0f;
    bool hasDiffuseMap = false;
    bool hasSpecularMap = false;
    bool hasEmissionMap = false;

    unsigned int diffuseMap;
    int diffuse;
    unsigned int specularMap;
    int specular;

    unsigned int emissionMap;
    int emission;

    glm::vec3 repeatTextureFactor = glm::vec3(1.0f);//this is mutiplied by the gameObjectScale
    bool repeatTexture = true;



    glm::vec3 ambient;

    float brightness;

    glm::vec4 color;


    unsigned int generateTexture(const char* path);
    
};
//void generateTexture(unsigned int& texture, const char* path);