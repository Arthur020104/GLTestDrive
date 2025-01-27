#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Input.h"
#include "main.h"


bool toggletWireFrameMode = false;
bool wireFrame = false;
bool scrolling = false;
bool scrollingUp = false;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// Se nao estiver pressionado, glfwGetKey retorna GLFW_RELEASE//if it's not pressed, glfwGetKey returns GLFW_RELEASE
    {
        glfwSetWindowShouldClose(window, true);
    }
    /***************WireFrame*************************/
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        toggletWireFrameMode = true;

    }//fazendo com que somente quando o botao eh solto o wireFrmae sofre toggle// Only when the button is realease the wireframe is toggle
    if (toggletWireFrameMode && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        toggletWireFrameMode = false;
        wireFrame = !wireFrame;
        wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {

    }
    scrolling = false;
    /***************WireFrame*************************/
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
{
    scrolling = true;
    if (yoffset > 0) 
    {
        scrollingUp = true;
    }
    else if (yoffset < 0)  
    {
        scrollingUp = false;
    }
}

bool isKeyPressed(GLFWwindow* window, const int& key)
{
    if (glfwGetKey(window, key) == GLFW_PRESS)
        return true;
    return false;
}