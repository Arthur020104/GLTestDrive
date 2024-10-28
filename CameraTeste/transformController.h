#pragma once
#include <glad/glad.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>


class TransformController
{
public:
	// Constructor
	TransformController(glm::vec3 initialPos, glm::vec3 initialRot): objPos(initialPos), objRot(initialRot) {updateModelMatrix();}
	glm::mat4 getModelMatrix();

	glm::vec3 forward();

	glm::vec3 right();

	glm::vec3 getPos();
	glm::vec3 getRot();
	glm::vec3 getScale();

	virtual void setPos(const glm::vec3& newPos);
	virtual void setRot(const glm::vec3& newRot);
	virtual void setScale(const glm::vec3& newScale);



private:
	glm::vec3 objPos;
	glm::vec3 objRot;
	glm::vec3 objScale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 forwardVec;
	glm::vec3 rightVec;

	glm::mat4 ModelMatrix;


	void updateModelMatrix();
};
