#pragma once
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "main.h"
class Camera
{
public:
	Camera(glm::vec3 inicialPos, glm::vec3 target, bool isFpsCam);

	Camera(glm::vec3 inicialPos, glm::vec3 target): Camera(inicialPos, target, false) {}

	Camera(glm::vec3 inicialPos, bool isFpsCam) : Camera(inicialPos, glm::vec3(0,0, FAR_FRUSTUM), isFpsCam) {}

	Camera(glm::vec3 inicialPos): Camera(inicialPos, glm::vec3(0, 0, FAR_FRUSTUM), false) {}

	Camera(): Camera(glm::vec3(0, 0, 6), glm::vec3(0, 0, FAR_FRUSTUM), false) {}

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
