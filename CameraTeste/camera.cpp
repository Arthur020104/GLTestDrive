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
	return glm::normalize(forwardVec);
}
glm::vec3 Camera::right()
{
	return  glm::normalize(rightVec);
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
	glm::vec3 direction = targetPos - cameraPos;

	glm::mat4 rotationMatrix = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(static_cast<float>(degrees)),
		glm::vec3(0, 1.0f, 0)
	);

	glm::vec4 rotatedDir = rotationMatrix * glm::vec4(direction, 0.0f);

	targetPos = cameraPos + glm::vec3(rotatedDir);

	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(forwardVec, glm::vec3(0, 1, 0)));
}

void Camera::verticalRotation(const double& degrees)
{
	glm::vec3 direction = targetPos - cameraPos;

	glm::mat4 rotationMatrix = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(static_cast<float>(degrees)),
		rightVec
	);

	glm::vec4 rotatedDir = rotationMatrix * glm::vec4(direction, 0.0f);
	glm::vec3 newDirection = glm::normalize(glm::vec3(rotatedDir));

	if (fabs(newDirection.y) > 0.99f)
		return;

	targetPos = cameraPos + glm::vec3(rotatedDir);

	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(forwardVec, glm::vec3(0, 1, 0)));
}

void Camera::setTarget(const glm::vec3& target)
{
	if (fpsCam)
	{
		glm::vec3 dir = glm::normalize(target - cameraPos);

		targetPos = cameraPos + dir * glm::fvec1(FAR_FRUSTUM);

		forwardVec = glm::normalize(targetPos - cameraPos);
		rightVec = glm::normalize(glm::cross(forwardVec, glm::vec3(0, 1, 0)));
		return;
	}

	targetPos = target;
	forwardVec = glm::normalize(targetPos - cameraPos);
	rightVec = glm::normalize(glm::cross(forwardVec, glm::vec3(0, 1, 0)));
}
