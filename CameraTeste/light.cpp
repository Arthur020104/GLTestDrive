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
        value = glm::normalize(this->getPos());
        return glm::length(value) > 0.0f ? value : glm::vec3(0.0f,0.0f,0.0f);
    }
    value = glm::normalize(this->getPos() - obj);
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
void Light::setModelMatrixMode(const unsigned short& newModelMatrixMode)
{
	TransformController::setModelMatrixMode(newModelMatrixMode);
	lightTransform.setModelMatrixMode(newModelMatrixMode);
	if (lightsThatWereUpdated != nullptr)
	{
		lightsThatWereUpdated->insert(this);
	}
}
void Light::prepareRender()
{
	GameObject::mustHaveRenderAtribb("prepareRender");//rendering the light body MAKE THIS OPTIONAL
	glBindVertexArray(VAO);
	Shader* shaderProgramL = this->getShaderPointer();
	shaderProgramL->use();

	this->getShaderPointer()->setVec3("LightColor", color);

	shaderProgramL->setMat4("model", this->getModelMatrix());

	//glBindVertexArray(VAO);
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
glm::vec3 Light::getAmbient()
{
	return this->ambient;
}
glm::vec3 Light::getDiffuse()
{
	return this->diffuse;
}
glm::vec3 Light::getSpecular()
{
	return this->specular;
}
void Light::setIntensity(float value)
{
	intensity = value;
	lightsThatWereUpdated->insert(this);
}
void Light::setAmbient(glm::vec3 amb)
{
	this->ambient = amb;
	lightsThatWereUpdated->insert(this);
}
void Light::setDiffuse(glm::vec3 diff)
{
	this->diffuse = diff;
	lightsThatWereUpdated->insert(this);
}
void Light::setSpecular(glm::vec3 spec)
{
	this->specular = spec;
	lightsThatWereUpdated->insert(this);
}