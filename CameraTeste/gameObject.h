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
#include "material.h"
#include "main.h"

// Constantes padrão
const bool DEFAULT_STATIC = true;
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);
/**TODO**
1. This class must be able to be the parent class of lights and DONE
    1. Must Work without necessarly needing to have a pyshical representationg on the world DONE
2.Create functions pointer for beforeUpdate and aftherUpdate
3. Make EBO OPTMIZATION
4. Load from data from files
    4.0 Constructor mus aceppt path file instead of vertices and vertices num
    4.1 load vertices from files method
    4.2 load normals from files method

**TODO**/
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

    GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot) 
        : TransformController(inicialPos, inicialRot), isStaticObj(DEFAULT_STATIC) {}

    GameObject() 
        : TransformController(DEFAULT_POSITION, DEFAULT_ROTATION), isStaticObj(DEFAULT_STATIC) {}

    // Destrutor
    ~GameObject();

    // Variáveis públicas
    glm::vec4 objectColor;

    // Métodos públicos
    bool hasVertices();

    void BeforeUpdate();
    void Update();
    void AftherUpdate();

    void setMaterial(Material* newMaterial);
    void setUpdateFunc(std::function<void(GameObject*)> func);
    void setAftherUpdateFunc(std::function<void(GameObject*)> func);
    virtual void prepareRender();
    void setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures);
    void updateNormals();
    int getVerticesNum();
    Shader* getShaderPointer() const;
    void enablePhysicalRepresentation(const float* vertices, const int& numVertices, Shader* shaderProgramRef);
    void disableTextures();
    bool hasRenderAtribs();
private:
    // Variáveis privadas
    std::vector<float> vertexNormals;
    int sizeOfVerticesObj = -1;
    float* verticesObj = nullptr;
    bool isStaticObj;
    std::vector<unsigned int> texturesIds;

    std::function<void(GameObject*)> updateFunc;
    std::function<void(GameObject*)> aftherUpdateFunc;
   
    Material* material = &DEFAULT_MATERIAL;

    unsigned int VAO;
    unsigned int VBO;
    Shader* shaderProgram = nullptr;
    int verticesNum = -1; // Defaults to -1 to throw an error if the object requires vertices but has none.

    // Métodos privados
   

protected:
    void mustHaveRenderAtribb(std::string methodName);
    unsigned int getVAO();
    unsigned int getVBO();
    std::vector<unsigned int>* getTextureIds();
};

// Declaração de funções
void generateTexture(unsigned int& texture, const char* path);
