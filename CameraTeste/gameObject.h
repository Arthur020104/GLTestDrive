#pragma once

// Bibliotecas padrão
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <functional>

// Bibliotecas do projeto
#include "transformController.h"
#include "shader.h"
#include "light.h"

// Constantes padrão
const glm::vec4 DEFAULT_COLOR = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
const bool DEFAULT_STATIC = true;
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);

class GameObject : public TransformController 
{
public:
    // Construtores
    GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const float* vertices, const int& numVertices, Shader* shaderProgramRef, const bool& isStatic);
    GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const float* vertices, const int& numVertices, Shader* shaderProgramRef)
        : GameObject(inicialPos, inicialRot, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}
    GameObject(const glm::vec3& inicialPos, const float* vertices, const int& numVertices, Shader* shaderProgramRef)
        : GameObject(inicialPos, DEFAULT_ROTATION, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}
    GameObject(const float* vertices, const int& numVertices, Shader* shaderProgramRef)
        : GameObject(DEFAULT_POSITION, DEFAULT_ROTATION, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}
    GameObject() : TransformController(DEFAULT_POSITION, DEFAULT_ROTATION), isStaticObj(DEFAULT_STATIC) {}

    // Destrutor
    ~GameObject();

    // Variáveis públicas
    glm::vec4 objectColor;

    // Métodos públicos
    bool hasVertices();
    void objUpdate();
    void setUpdateFunc(std::function<void(GameObject*)> func);
    void prepareRender();
    void setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures);
    void updateNormals();
    int getVerticesNum();
    Shader* getShaderPointer() const;

private:
    // Variáveis privadas
    std::vector<float> vertexNormals;
    int sizeOfVerticesObj = -1;
    float* verticesObj = nullptr;
    bool isStaticObj;
    std::vector<unsigned int> texturesIds;
    std::function<void(GameObject*)> updateFunc;

    unsigned int VAO;
    unsigned int VBO;
    Shader* shaderProgram = nullptr;
    int verticesNum = -1; // Defaults to -1 to throw an error if the object requires vertices but has none.

    // Métodos privados
    void mustHaveRenderAtribb(std::string methodName);
};

// Declaração de funções
void generateTexture(unsigned int& texture, const char* path);
