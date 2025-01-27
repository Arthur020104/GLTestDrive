#pragma once

// Biblioteca GLM
#include <glm/glm/gtc/matrix_transform.hpp>
#include <set>
// Biblioteca do projeto
#include "material.h"
#include "transformController.h"
#include "gameObject.h"

const glm::vec3 DEFAULT_INICIAL_POS = glm::vec3(0.0f);
const glm::vec3 DEFAULT_INICIAL_ROT = glm::vec3(0.0f);
const glm::vec3 DEFAULT_LIGHT_COLOR = glm::vec3(0.3f);
const bool DEFAULT_DIRECTIONAL = false;
const float DEFAULT_INTENSITY = 1.0f;
const float DEFAULT_SPECULAR_FACTOR = 1.0f;
const float DEFAULT_AMBIENT_FACTOR = 0.4f;
const float DEFAULT_DIFFUSE_FACTOR = 1.0f;
const TransformController DEFAULT_LIGHT_TRANSFORM = TransformController(DEFAULT_INICIAL_POS, DEFAULT_INICIAL_ROT);

class Light : public GameObject
{
public:
    // Construtores
    Light(const glm::vec3& inicialPos = DEFAULT_INICIAL_POS,const glm::vec3& inicialRot = DEFAULT_INICIAL_ROT,const glm::vec3& color = DEFAULT_LIGHT_COLOR,bool isDirectional = DEFAULT_DIRECTIONAL,float intensity = DEFAULT_INTENSITY,const glm::vec3& ambient = glm::vec3(-1.0f),const glm::vec3& diffuse = glm::vec3(-1.0f), const glm::vec3& specular = glm::vec3(-1.0f)) :GameObject(inicialPos, inicialRot), color(color), isDirectional(isDirectional), intensity(intensity), ambient(ambient.x < 0 ? DEFAULT_AMBIENT_FACTOR * color : ambient), diffuse(diffuse.x < 0 ? DEFAULT_DIFFUSE_FACTOR * color : diffuse), specular(specular.x < 0 ? DEFAULT_SPECULAR_FACTOR * color: specular), lightTransform(TransformController(inicialPos, inicialRot)) {}

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
            ambient = other.ambient;
            diffuse = other.diffuse;
            specular = other.specular;
        }
        return *this;
    }

    void setPos(const glm::vec3& newPos) override;
    void setRot(const glm::vec3& newRot) override;
    void setColor(const glm::vec3& newColor);
    void setModelMatixMode(const unsigned short& newModelMatrixMode) override;
    //  void setScale(const glm::vec3& newScale) override;
    void prepareRender() override;

    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();

    glm::mat4 getLightModelMat();
    std::set<Light*>* lightsThatWereUpdated = nullptr; //This is going to be a pointer to the set in the scene class

    TransformController lightTransform;

private:
    // Atributos privados
    glm::vec3 color;
    bool isDirectional;
    float intensity;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
