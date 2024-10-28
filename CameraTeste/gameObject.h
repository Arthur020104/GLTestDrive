#pragma once
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>
#include "transformController.h"
#include "shader.h"
#include "light.h"
#include <functional>

const glm::vec4 DEFAULT_COLOR = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
const bool DEFAULT_STATIC = true;
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);
class GameObject:public TransformController
{
public:
	unsigned int VAO;
	unsigned int VBO;
	Shader* shaderProgram;
	int verticesNum;

	glm::vec4 objectColor;

	//unsigned int EBO;//can be automatcly created do it later, use a static method of gameobject

	GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot,const glm::vec4 &color, const float* vertices, const int& numVertices, Shader* shaderProgramRef, const bool& isStatic);
	//Camera(glm::vec3 inicialPos, bool isFpsCam) : Camera(inicialPos, glm::vec3(0, 0, FAR_FRUSTUM), isFpsCam) {}
	GameObject(const glm::vec3& inicialPos, const glm::vec3& inicialRot, const float* vertices, const int& numVertices, Shader* shaderProgramRef, const bool& isStatic) : GameObject(inicialPos, inicialRot, DEFAULT_COLOR, vertices, numVertices, shaderProgramRef, isStatic) {}

	GameObject(const glm::vec3 &inicialPos,const glm::vec3 &inicialRot, const float* vertices, const int& numVertices, Shader* shaderProgramRef): GameObject(inicialPos,inicialRot,DEFAULT_COLOR, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}

	GameObject(const glm::vec3 &inicialPos, const float* vertices, const int& numVertices, Shader* shaderProgramRef) : GameObject(inicialPos, DEFAULT_ROTATION, DEFAULT_COLOR, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}

	GameObject(const float* vertices, const int& numVertices, Shader* shaderProgramRef) : GameObject(DEFAULT_POSITION, DEFAULT_ROTATION, DEFAULT_COLOR, vertices, numVertices, shaderProgramRef, DEFAULT_STATIC) {}


	~GameObject();

	void objUpdate();
	void setUpdateFunc(std::function<void(GameObject*)> func);

	void prepareRender(glm::mat4& view, const glm::mat4& projectio);
	void setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures);
	
	void updateLighting(Light* l, const int &sizeOfLightArr);

	
	void updateNormals();

	void recalculateWVectors();
private:

	std::vector<float> vertexNormals;
	
	int sizeOfVerticesObj;
	float* verticesObj;

	Light* lightsArr;
	int sizeOfLights;
	std::vector<glm::vec3> wValues;
	/*This will be changed will be values of the scene not gameobject*/
	std::vector<glm::vec3> lightColors;
	std::vector<float> lightIntensity;
	std::vector<glm::mat3> lightModelMatrix;
	/*This will be changed will be values of the scene not gameobject*/

	bool isStaticObj;

	std::vector<unsigned int> texturesIds;
	
	
	std::function<void(GameObject*)> updateFunc;

	void applyLighting();


};
