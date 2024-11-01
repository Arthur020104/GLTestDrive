#pragma once   
#include <GLFW/glfw3.h>  
#include "shader.h"
#include "material.h"
extern float DeltaTime;
extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;
extern const double NEAR_FRUSTUM;
extern const double FAR_FRUSTUM;
extern Material DEFAULT_MATERIAL;

extern Shader* DEFAULT_SHADER_REFERENCE;

// Function prototypes
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void printVec3(const glm::vec3& vec);
void fpsLog();
