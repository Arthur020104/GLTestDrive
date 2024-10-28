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