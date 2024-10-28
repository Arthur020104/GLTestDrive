#pragma once
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "main.h"
const glm::vec3 DEFAULT_TARGET = glm::vec3(0, 0, FAR_FRUSTUM);
class Camera
{
public:
	Camera(glm::vec3 inicialPos, glm::vec3 target, bool isFpsCam);

	Camera(glm::vec3 inicialPos, glm::vec3 target): Camera(inicialPos, target, false) {}

	Camera(glm::vec3 inicialPos, bool isFpsCam) : Camera(inicialPos, DEFAULT_TARGET, isFpsCam) {}

	Camera(glm::vec3 inicialPos): Camera(inicialPos, DEFAULT_TARGET, false) {}

	Camera(): Camera(glm::vec3(0, 0, 6), DEFAULT_TARGET, false) {}

	glm::vec3 foward();

	glm::vec3 right();

	glm::vec3 getPos();

	void setPos(const glm::vec3& newPos);


	void setTarget(const glm::vec3& target);

	void horizontalRotation(const double& degrees);
	void verticalRotation(const double& degrees);

	glm::mat4 getViewMatrix();
	//~Camera();
	
private:
	glm::vec3 cameraPos;
	glm::vec3 targetPos; 
	glm::vec3 forwardVec;
	glm::vec3 rightVec;
	bool fpsCam = false;
};
