#pragma once
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "camera.h"
#include <iostream>
#include "main.h"

Camera::Camera(glm::vec3 inicialPos, glm::vec3 target, bool isFpsCam)
{
	cameraPos = inicialPos;
	targetPos = target;
	fpsCam = isFpsCam;
	if (fpsCam && targetPos.z < cameraPos.z)
	{
		targetPos.z = cameraPos.z+ 1;
	}


	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(foward(), glm::vec3(0, 1, 0)));
}
glm::vec3 Camera::foward()
{ 
	return forwardVec;
}
glm::vec3 Camera::right()
{
	return  rightVec;
}
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, targetPos, glm::vec3(0, 1, 0));
}

glm::vec3 Camera::getPos()
{
	return cameraPos;
}
void Camera::setPos(const glm::vec3& newPos)
{
	if (fpsCam)
	{
		glm::vec3 diff = newPos - cameraPos;
		targetPos += diff;
		cameraPos = newPos;
		return;
	}
	cameraPos = newPos;

	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(foward(), glm::vec3(0, 1, 0)));
}

void Camera::horizontalRotation(const double& degrees) 
{
	glm::vec4 aux = glm::vec4(targetPos, 1.0f);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(degrees)), glm::vec3(0, 1.0f, 0));

	aux = rotationMatrix * aux;

	targetPos.x = aux.x;
	targetPos.y = aux.y;
	targetPos.z = aux.z;

	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(foward(), glm::vec3(0, 1, 0)));
}
void Camera::verticalRotation(const double& degrees)
{
	
	glm::vec4 aux = glm::vec4(targetPos, 1.0f);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(degrees)), glm::vec3(forwardVec.z * -1, 0, forwardVec.x));

	aux = rotationMatrix * aux;
	
	if ((glm::normalize(cameraPos - glm::vec3(aux.x, aux.y, aux.z)).y >= 0.999 && aux.y <= targetPos.y) || (glm::normalize(cameraPos - glm::vec3(aux.x, aux.y, aux.z)).y <= -0.999 && aux.y >= targetPos.y))
	{
		return;
	}

	targetPos.x = aux.x;
	targetPos.y = aux.y;
	targetPos.z = aux.z;

	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(foward(), glm::vec3(0, 1, 0)));
}