#pragma once

// Bibliotecas de terceiros
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <set>
#include <vector>

// Bibliotecas do projeto
#include "light.h"
#include "gameObject.h"
#include "camera.h"

// Definição de constantes
const int MAX_LIGHTS = 10;

class Scene
{
public:
    // Construtor da classe Scene
    Scene(GameObject** objArr, const int& sizeObjArr, Camera* camera, const glm::mat4& projection);

    // Métodos de gerenciamento de cena
    void render();

private:
    // Vetor de objetos da cena
    std::vector<GameObject*> sceneObjs;

    // Array de luzes da cena
    std::vector<Light*> sceneLights;

    // Câmera principal
    Camera* mainCamera;

    // Matriz de projeção
    glm::mat4 projectionMat;

    // Caches para propriedades das luzes
    std::vector<glm::vec3> lightsPosWorldCache;
    std::vector<glm::vec3> lightsColorsCache;
    std::vector<glm::vec3> lightsSpecularCache;
    std::vector<glm::vec3> lightsDiffuseCache;
    std::vector<glm::vec3> lightsAmbientCache;
    std::vector<float> lightsIntensityCache;

    std::set<Light*> lightsChanged;

    void updateLightsCache();//should be call when all cache elements needs to be updated, call it when add new object
    void groupObjVectorByShader();
    void updateLightsChangedCache();//should be call when a element on cache is changed
    void updateElementCache(Light* element);
};
