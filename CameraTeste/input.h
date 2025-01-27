#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

extern bool scrolling;
extern bool scrollingUp;

void processInput(GLFWwindow* window);
bool isKeyPressed(GLFWwindow* window, const int& key);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);