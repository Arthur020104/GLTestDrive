#pragma once
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shader.h"
#include <functional>
class GameObject
{
public:
	unsigned int VAO;
	unsigned int VBO;
	Shader* shaderProgram;
	int verticesNum;
	//unsigned int EBO;//can be automatcly created do it later, use a static method of gameobject

	GameObject(glm::vec3 inicialPos, glm::vec3 inicialRot, const float* vertices, const int& sizeOfVertices, Shader* shaderProgramRef);


	~GameObject();
	glm::vec3 forward();

	glm::vec3 right();

	glm::vec3 getPos();
	glm::vec3 getRot();
	glm::vec3 getScale();

	void setPos(const glm::vec3& newPos);
	void setRot(const glm::vec3& newRot);
	void setScale(const glm::vec3& newScale);

	void objUpdate();
	void setUpdateFunc(std::function<void(GameObject*)> func);

	glm::mat4 getModelMatrix();

	void prepareRender();
	void setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures);
	

private:
	glm::vec3 objPos;
	glm::vec3 objRot;
	glm::vec3 objScale = glm::vec3(1.0f,1.0f,1.0f);

	std::function<void(GameObject*)> updateFunc;

	glm::vec3 forwardVec;
	glm::vec3 rightVec;
	std::vector<unsigned int> texturesIds;//add vector of bleding value
	int sizeOfVerticesObj;
	float *verticesObj;
};
