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
const float DEFAULT_CUT_OFF = 12.5f;//this is not what will be stored in the light, we store this glm::cos(glm::radians(DEFAULT_CUT_OFF))
const float DEFAULT_OUTER_CUT_OFF = 17.5f;
class Light : public GameObject
{
public:
    // Construtores
    Light(const glm::vec3& inicialPos = DEFAULT_INICIAL_POS, const glm::vec3& inicialRot = DEFAULT_INICIAL_ROT, const glm::vec3& color = DEFAULT_LIGHT_COLOR,
        const bool isDirectional = DEFAULT_DIRECTIONAL,const float intensity = DEFAULT_INTENSITY, const bool isPointL = false , const float iCutOFF = DEFAULT_CUT_OFF,
        const float outerCutOffi = DEFAULT_OUTER_CUT_OFF, const glm::vec3& ambient = glm::vec3(-1.0f), const glm::vec3& diffuse = glm::vec3(-1.0f),
        const glm::vec3& specular = glm::vec3(-1.0f))
        :
        GameObject(inicialPos, inicialRot), color(color), isDirectional(isDirectional), intensity(intensity), ambient(ambient.x < 0 ? DEFAULT_AMBIENT_FACTOR * color : ambient),
        diffuse(diffuse.x < 0 ? DEFAULT_DIFFUSE_FACTOR * color : diffuse), specular(specular.x < 0 ? DEFAULT_SPECULAR_FACTOR * color : specular),
        lightTransform(TransformController(inicialPos, inicialRot)), isPoint(isPointL), cutOff(glm::cos(glm::radians(iCutOFF))), outerCutOff(glm::cos(glm::radians(outerCutOffi))),
        direction(this->getForward())
        {}

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

    bool getIsDirectional() const { return this->isDirectional; };

    glm::mat4 getLightModelMat();
    std::set<Light*>* lightsThatWereUpdated = nullptr; //This is going to be a pointer to the set in the scene class

    TransformController lightTransform;

    void setIntensity(float value);
    float getIntensity() const { return this->intensity; }

    void setAmbient(glm::vec3 amb);
    void setDiffuse(glm::vec3 diff);
    void setSpecular(glm::vec3 spec);


    glm::vec3 getDirection() { return this->direction;  };
    void setDirection(glm::vec3 directionv) { this->direction = directionv; };
    float getCutOff() const { return this->isPoint ? this->cutOff : -1.0f; };

    float getOuterCutOff() const{ return this->isPoint ? this->outerCutOff : -1.0f; };

private:
    // Atributos privados
    glm::vec3 color;

    bool isDirectional;
    float intensity;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
   

    //Point Light, maybe create another class. First finish model loading
    float cutOff;
    float outerCutOff;
    bool isPoint;
    glm::vec3 direction;
};
