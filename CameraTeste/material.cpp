#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "material.h"
#include "Libs/stb_image.h"
#include <iostream>



void Material::LoadMaterialDataToShader(Shader* s)
{
    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != s->ID)
    {
        std::cerr << "Error: Shader program not active before loading material data.\n";
        return;
    }

    s->setVec4("material.color", this->color);
    s->setFloat("material.roughness", this->brightness);
    s->setFloat("material.diffuseMulti", this->diffuseMulti);
    s->setFloat("material.specularMulti", this->specularMulti);
    s->setFloat("material.emissionMulti", this->emMulti);

    s->setBool("material.hasSpecularMap", this->hasSpecularMap);
    s->setBool("material.hasDiffuseMap", this->hasDiffuseMap);
    s->setBool("material.hasEmissionMap", this->hasEmissionMap);

    s->setInt("material.specular", this->specular);

    s->setInt("material.diffuse", diffuse);
    s->setInt("material.emission", emission);

    s->setBool("material.repeatTexture", this->repeatTexture);


    s->setVec3("material.repeatTextureFactor", this->repeatTextureFactor);


    glActiveTexture(GL_TEXTURE0 + diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE0 + specular);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glActiveTexture(GL_TEXTURE0 + emission);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    s->setVec3("material.ambient", this->ambient);
}

void Material::LoadMaterialDataToShader(Shader* s, glm::vec3 scale)
{
    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != s->ID)
    {
        std::cerr << "Error: Shader program not active before loading material data.\n";
        return;
    }

    s->setVec4("material.color", this->color);
    s->setFloat("material.brightness", this->brightness);
    s->setFloat("material.diffuseMulti", this->diffuseMulti);
    s->setFloat("material.specularMulti", this->specularMulti);
    s->setFloat("material.emissionMulti", this->emMulti);

    s->setBool("material.hasSpecularMap", this->hasSpecularMap);
    s->setBool("material.hasDiffuseMap", this->hasDiffuseMap);
    s->setBool("material.hasEmissionMap", this->hasEmissionMap);

    s->setInt("material.specular", this->specular);

    s->setInt("material.diffuse", diffuse);
    s->setInt("material.emission", emission);

    s->setBool("material.repeatTexture", this->repeatTexture);

 
    s->setVec3("material.repeatTextureFactor", this->repeatTextureFactor * scale);


    glActiveTexture(GL_TEXTURE0 + diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE0 + specular);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glActiveTexture(GL_TEXTURE0 + emission);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    s->setVec3("material.ambient", this->ambient);
}
void Material::UnbindMaterial()
{
    glActiveTexture(GL_TEXTURE0 + diffuse);
    glBindTexture(GL_TEXTURE_2D, 0);


    glActiveTexture(GL_TEXTURE0 + specular);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + emission);
    glBindTexture(GL_TEXTURE_2D, 0);
}
unsigned int Material::generateTexture(const char* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cerr << "Error: Could not load texture: " << path << std::endl;
        glDeleteTextures(1, &texture);
        return 0;
    }

    std::cout << "Successfully loaded texture: " << path
        << " (" << width << "x" << height << ", "
        << nrChannels << " channels)" << std::endl;

    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 2) format = GL_RG;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    else 
    {
        std::cerr << "Unsupported texture format: " << nrChannels << " channels.\n";
        stbi_image_free(data);
        glDeleteTextures(1, &texture);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void Material::LoadDiffuseMap(const char* path)
{
    unsigned int texture = generateTexture(path);
    if (texture == 0)
    {
        std::cerr << "Error: Failed to load diffuse map from path: " << path << std::endl;
    }
    else
    {
        this->diffuseMap = texture;
        hasDiffuseMap = true;
    }
   
}
void Material::LoadSpecularMap(const char* path)
{
    unsigned int textureS = generateTexture(path);
    if (textureS == 0)
    {
        std::cerr << "Error: Failed to load specular map from path: " << path << std::endl;
    }
    else
    {
        this->specularMap = textureS;
        hasSpecularMap = true;
    }
}
void Material::LoadEmissionMap(const char* path)
{
    unsigned int texture = generateTexture(path);
    if (texture == 0)
    {
        std::cerr << "Error: Failed to load emission map from path: " << path << std::endl;
    }
    else
    {
        this->emissionMap = texture;
        hasEmissionMap = true;
    }
}
