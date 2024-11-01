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
    Scene(GameObject** objArr, const int& sizeObjArr, Light** lightArr, const int& sizeLightArr, Camera* camera, const glm::mat4& projection);

    // Métodos de gerenciamento de cena
    void render();

private:
    // Vetor de objetos da cena
    std::vector<GameObject*> sceneObjs;

    // Array de luzes da cena
    Light* sceneLights[MAX_LIGHTS];
    int sizeOfLightsArr = 0;

    // Câmera principal
    Camera* mainCamera;

    // Matriz de projeção
    glm::mat4 projectionMat;

    // Caches para propriedades das luzes
    std::vector<glm::vec3> lightsPosWorldCache;
    std::vector<glm::vec3> lightsColorsCache;
    std::vector<float> lightsIntensityCache;

    std::set<Light*> lightsChanged;

    void updateLightsCache();
    void groupObjVectorByShader();
};
