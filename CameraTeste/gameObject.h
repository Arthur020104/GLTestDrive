#pragma once
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shader.h"
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

	void setPos(const glm::vec3& newPos);
	void setRot(const glm::vec3& newRot);

	glm::mat4 getModelMatrix();

	void prepareRender();
	void setTextures(const float* textureCoords, const int& sizeOftextureCoords, const char** paths, const int& numberOfTextures);
	

private:
	glm::vec3 objPos;
	glm::vec3 objRot;
	glm::vec3 forwardVec;
	glm::vec3 rightVec;
	std::vector<unsigned int> texturesIds;//add vector of bleding value
	int sizeOfVerticesObj;
	float *verticesObj;
};
