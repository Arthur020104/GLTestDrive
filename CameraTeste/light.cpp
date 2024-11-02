#include <glm/glm/gtc/matrix_transform.hpp>
#include "light.h"
bool containsNaN3(const glm::vec3& vec)
{
	return std::isnan(vec.x) || std::isnan(vec.y) || std::isnan(vec.z);
}
glm::vec3 Light::getW(const glm::vec3& obj)
{
	glm::vec3 value;

	if (isDirectional)
	{
		value =  glm::normalize(this->getPos());
		return glm::length(value) > 0.0f ? value : glm::vec3(0.0f,0.0f,0.0f);
	}
	value = glm::normalize(this->getPos()- obj);

	return glm::length(value) > 0.0f ? value : glm::vec3(0.0f, 0.0f, 0.0f);
}
glm::vec3 Light::getColor()
{
	return color;
}
float Light::getIntensity()
{
	return intensity;
}
void Light::setPos(const glm::vec3& newPos)
{
	TransformController::setPos(newPos);
	lightTransform.setPos(newPos);
	if (lightsThatWereUpdated != nullptr)
	{
		lightsThatWereUpdated->insert(this);
	}
}
void Light::setRot(const glm::vec3& newRot)
{
	TransformController::setRot(newRot);
	lightTransform.setRot(newRot);
	if (lightsThatWereUpdated != nullptr)
	{
		lightsThatWereUpdated->insert(this);
	}
}
void Light::setColor(const glm::vec3& newColor)
{
	color = newColor;
	if (lightsThatWereUpdated != nullptr)
	{
		lightsThatWereUpdated->insert(this);
	}
}
/*void Light::setScale(const glm::vec3& newScale)
{
	TransformController::setScale(newScale);
	if (lightsThatWereUpdated != nullptr)
	{
		lightsThatWereUpdated->insert(this);
	}
}*/
void Light::prepareRender()
{
	GameObject::mustHaveRenderAtribb("prepareRender");

	Shader* shaderProgramL = this->getShaderPointer();
	shaderProgramL->use();

	this->getShaderPointer()->setVec3("LightColor", color);

	shaderProgramL->setMat4("model", this->getModelMatrix());

	std::vector<unsigned int>* texturesIds = this->getTextureIds();

	for (int i = 0; i < texturesIds->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texturesIds->at(i));
	}
	glBindVertexArray(this->getVAO());
}
/*
Light::~Light()
{
	return;
}*/
glm::mat4 Light::getLightModelMat()
{
	return lightTransform.getModelMatrix();
}