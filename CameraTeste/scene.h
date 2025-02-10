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

    struct LightsCache
    {
        glm::vec3 posWorldCache;
        glm::vec3 colorsCache;
        glm::vec3 specularCache;
        glm::vec3 diffuseCache;
        glm::vec3 ambientCache;
        float intensityCache;
        bool directionalCache;

        glm::vec3 directionCache;
        float cutOffCache;
    };

    // Instância da estrutura LightsCache
    std::vector<LightsCache> lightsCache;
    
    

    std::set<Light*> lightsChanged;

    void updateLightsCache();//should be call when all cache elements needs to be updated, call it when add new object
    void groupObjVectorByShader();
    void updateLightsChangedCache();//should be call when a element on cache is changed
    void updateElementCache(Light* element);
};
