#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>

const float DEFAULT_ROUGHNESS = 500.0f;

const float DEFAULT_SPECULAR_AMOUNT = 0.5f;

const glm::vec4 DEFAULT_COLOR = glm::vec4(0.3f, 0.3f, 0.5f, 1.0f);

class Material
{
public:
	float roughness;

	float amountOfSpecular;

	glm::vec4 color;

	Material(const glm::vec4& colorV, const float& roughnessAmt, const float& amtOfSpecular): color(colorV), roughness(roughnessAmt), amountOfSpecular(amtOfSpecular) {}

	Material(const float& roughnessAmt, const float& amtOfSpecular) : color(DEFAULT_COLOR), roughness(roughnessAmt), amountOfSpecular(amtOfSpecular) {}

	Material(const float& amtOfSpecular) : color(DEFAULT_COLOR), roughness(DEFAULT_ROUGHNESS), amountOfSpecular(amtOfSpecular) {}

	Material(const glm::vec4& colorV) : color(colorV), roughness(DEFAULT_ROUGHNESS), amountOfSpecular(DEFAULT_SPECULAR_AMOUNT) {}

	Material(const glm::vec4& colorV, const float& roughnessAmt) : color(colorV), roughness(roughnessAmt), amountOfSpecular(DEFAULT_SPECULAR_AMOUNT) {}

	Material() : color(DEFAULT_COLOR), roughness(DEFAULT_ROUGHNESS), amountOfSpecular(DEFAULT_SPECULAR_AMOUNT) {}

private:

};
