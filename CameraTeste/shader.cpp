#pragma once	
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


void Shader::checkCompileErrors(unsigned int Id, std::string type)
{
	int sucess;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(Id, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetShaderInfoLog(Id, 512, NULL, infoLog);
			std::cout << "ERROR::" << type << "SHADER COMPILATION FAILED" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(Id, GL_LINK_STATUS, &sucess);
		if (!sucess)
		{
			glGetProgramInfoLog(Id, 512, NULL, infoLog);
			std::cout << "ERROR::" << type << " LINKIN THE PROGRAM FAILED" << infoLog << std::endl;
		}
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//reading from shader files
	std::string vertexShader;
	std::string fragShader;
	try
	{
		vertexShader = readFile(vertexPath);
		fragShader = readFile(fragmentPath);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Erro: " << e.what() << "\n";
	}
	const char* vertexShaderCode = vertexShader.c_str();
	const char* fragShaderCode = fragShader.c_str();

	unsigned int vertexShaderID;
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID, "VERTEX");

	unsigned int fragShaderID;
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShaderID, 1, &fragShaderCode, NULL);
	glCompileShader(fragShaderID);
	checkCompileErrors(fragShaderID, "FRAGMENT");


	unsigned int programID;
	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);

	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);
	checkCompileErrors(programID, "PROGRAM");

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragShaderID);

	ID = programID;

}
void Shader::use()
{
	glUseProgram(ID);
}
/*void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setMat4(const std::string& name, const glm::mat4& value);?*/
void Shader::setBool(const std::string& name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, const  glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMat3(const std::string& name, const  glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const  glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setArrayVec3(const std::string& name, const glm::vec3* value, const GLsizei& size)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), size, glm::value_ptr(value[0]));
}
void Shader::setArrayVec4(const std::string& name, const glm::vec4* value, const GLsizei& size)
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), size, glm::value_ptr(value[0]));
}
void Shader::setArrayMat3(const std::string& name, const glm::mat3* value, const GLsizei& size)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), size, GL_FALSE, glm::value_ptr(value[0]));
}

void Shader::setVec4(const std::string& name, const  glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z,value.w);
}
void Shader::setFloatArray(const std::string& name, const float* arr, const GLsizei& size)
{
	glUniform1fv(glGetUniformLocation(ID, name.c_str()), size, arr);
}
void Shader::setIntArray(const std::string& name, const int* arr, const GLsizei& size)
{
	glUniform1iv(glGetUniformLocation(ID, name.c_str()), size, arr);
}

std::string readFile(const std::string& path)
{
	std::ifstream file(path);


	if (!file.is_open())
	{
		throw std::runtime_error("[ERROR] Could not open file at " + path + "\n");
	}

	std::string fileContent;
	while (file)
	{
		std::string line;
		std::getline(file, line);

		fileContent.append(line + "\n");

	}
	file.close();

	return fileContent;
}
