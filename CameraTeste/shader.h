#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

std::string readFile(const std::string& path);
class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setMat4(const std::string& name, const glm::mat4& value);
	void setVec2(const std::string& name, const glm::vec2& value);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setVec4(const std::string& name, const glm::vec4& value);
	void setFloatArray(const std::string&name, const float* arr, const GLsizei& sizeArr);
	void setIntArray(const std::string& name, const int* arr, const GLsizei& size);
	void setArrayVec3(const std::string& name, const glm::vec3* value, const GLsizei& size);
	void setArrayVec4(const std::string& name, const glm::vec4* value, const GLsizei& size);
	void setMat3(const std::string& name, const  glm::mat3& value);
	void setArrayMat3(const std::string& name, const glm::mat3* value, const GLsizei& size);
private:
	void checkCompileErrors(unsigned int Id, std::string type);
};
