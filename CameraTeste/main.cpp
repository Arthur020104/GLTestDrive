
#pragma once
#include <glad/glad.h>
#include "main.h"
#include "shader.h"
#include "input.h"
#include "shapes.h"
#include "camera.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "gameObject.h"
#include <iostream>
//Callback function for when window is resized
//Funcao de callback para quando a janela eh resized(redimensionada)

double DeltaTime = 0;
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

const double NEAR_FRUSTUM = 0.1f;
const double FAR_FRUSTUM = 100.0f;

GLFWwindow* Window;
Shader* DEFAULT_SHADER_REFERENCE;

unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
unsigned int Texture1, Texture2;

double LastTime = 0;

float Speed = 8.0f;
float DefaultSpeed = 8.0f;
float VerticalSens = 100.0f;
float HorizontalSens = 100.0f;

int const AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 30;

glm::mat4 View;

Camera cameraInst = Camera(glm::vec3(0,0,-6), true);



const glm::mat4 PROJECTION = glm::perspective(glm::radians(103.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);

double LastXPos = -100000, LastYPos = -100000;
void printVec3(const glm::vec3& vec) {
    std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

void objUpdate(GameObject *obj)
{
    if (isKeyPressed(Window, GLFW_KEY_Z) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0, 0, Speed * 40 * DeltaTime));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Z))
    {
        obj->setScale(obj->getScale() + glm::vec3(0, 0, Speed * 1 * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_X) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(Speed * 40 * DeltaTime, 0, 0));
    }
    else if (isKeyPressed(Window, GLFW_KEY_X))
    {
        obj->setScale(obj->getScale() + glm::vec3(Speed * 1 * DeltaTime, 0, 0));
    }

    if (isKeyPressed(Window, GLFW_KEY_Y) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0, Speed * 40 * DeltaTime, 0));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Y))
    {
        obj->setScale(obj->getScale() + glm::vec3(0, Speed * 1 * DeltaTime, 0));
    }
    
    
    
}

void keyInput()
{
    
    if (isKeyPressed(Window, GLFW_KEY_S))
    {
        glm::vec3 newPos = cameraInst.getPos() + glm::fvec1((double)-Speed * DeltaTime) * cameraInst.foward();
        cameraInst.setPos(newPos);
    }
    if (isKeyPressed(Window, GLFW_KEY_W))
    {
        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1((double)Speed * DeltaTime) * cameraInst.foward();
        cameraInst.setPos(newPos);
       
    }
    if (isKeyPressed(Window, GLFW_KEY_D))
    {

        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1((double)Speed * DeltaTime) * cameraInst.right();
        cameraInst.setPos(newPos);
    }
    if (isKeyPressed(Window, GLFW_KEY_A))
    {
        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1((double)-Speed * DeltaTime) * cameraInst.right();
        cameraInst.setPos(newPos);
        

    }
    if (isKeyPressed(Window, GLFW_KEY_E))
    {
        cameraInst.setPos(cameraInst.getPos() + glm::vec3(0, Speed * DeltaTime, 0));
    }
    if (isKeyPressed(Window, GLFW_KEY_Q))
    {
        cameraInst.setPos(cameraInst.getPos() + glm::vec3(0, -Speed * DeltaTime, 0));
    }
    if (Speed <= DefaultSpeed && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        Speed *= 4;
    }
    else
    {
        Speed = DefaultSpeed;
    }

}
void cameraMovement()
{
    double xpos, ypos;
    glfwGetCursorPos(Window, &xpos, &ypos);

    if (LastXPos == -100000 || LastYPos == -100000)
    {
        LastXPos = xpos; LastYPos = ypos;
        return;
    }

    cameraInst.horizontalRotation(HorizontalSens* DeltaTime* (LastXPos - xpos));
    cameraInst.verticalRotation(HorizontalSens * DeltaTime * (LastYPos - ypos));
    LastXPos = xpos; LastYPos = ypos;
}
int init(GLFWwindow** window)
{
    if (!glfwInit())
    {
        printf("Failed to init glfw\n");
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need    Mac OS X you need to add glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); to your initialization code for it to work*/
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpengl", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(*window);
    //Avisando ao GLFW que queremos que essa funcao seja chamada toda vez que a janela eh redimensionada
    glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    /*******************Set GLFW********************/
    /*******************Set GLAD********************/

    //O Glad gerencia ponteiros para funcoes do OpenGL, entao precisamos inicializar o GLAD antes the usarmos funcoes do OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))/*GLFW nos da glfwGetProcAddress que define a funcao correta baseado em qual sistema operacional estamos compilando para .*/
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    /*******************Set GLAD********************/

    glViewport(0, 0, WIDTH, HEIGHT);
    return 0;
}
/*******************/
int main()
{

    if (init(&Window) == -1)
    {
        return -1;
    }
    
  
    float vertices[] = {
        // Posições
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    float texture[] = {
        // Coordenadas de textura
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };
    
    Shader DefaultShader("Shaders/vertexShaderDefault.glsl", "Shaders/fragmentShaderDefault.glsl");
    DEFAULT_SHADER_REFERENCE = &DefaultShader;
    GameObject obj = GameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), vertices, sizeof(vertices), DEFAULT_SHADER_REFERENCE);
    
    obj.setUpdateFunc(&objUpdate);

    const char* paths[] =
    {
    "TextureImages/scene.jpg",
    //"TextureImages/awesomeface.png"
    };
    obj.setTextures(texture, sizeof(texture), paths, 1);

    GameObject obj2 = GameObject(glm::vec3(2, 1, 0), glm::vec3(0, 0, 0), vertices, sizeof(vertices), DEFAULT_SHADER_REFERENCE);

    const char* paths2[] =
    {
    "TextureImages/container.jpg",
    };
    obj2.setTextures(texture, sizeof(texture), paths2, 1);

    GameObject obj3 = GameObject(glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0, 0, 0), vertices, sizeof(vertices), DEFAULT_SHADER_REFERENCE);

    const char* paths3[] =
    {
    "TextureImages/awesomeface.png",
    };
    obj3.setTextures(texture, sizeof(texture), paths3, 1);

    GameObject obj4 = GameObject(glm::vec3(2.0f, -0.5f, 0), glm::vec3(0, 0, 0), vertices, sizeof(vertices), DEFAULT_SHADER_REFERENCE);

    const char* paths4[] =
    {
    "TextureImages/awesomeface.pngs",
    "TextureImages/container.jpg"

    };
    obj4.setTextures(texture, sizeof(texture), paths4, 2);
    glEnable(GL_DEPTH_TEST);




    int FpsSampleCounter = 0;
    double DeltatimeMean = 0;
    // Mean for every 2 frames with weights 0.8 and 0.2, with the newer frame being weighted higher.
    while (!glfwWindowShouldClose(Window))
    {
        processInput(Window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture2);



        double timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;



        DeltatimeMean = (DeltatimeMean * 0.2 + DeltaTime * 0.8) / 2;
        FpsSampleCounter++;
        if (FpsSampleCounter >= AMOUNT_OF_SAMPLES_TO_COUNT_FPS)
        {
            FpsSampleCounter = 0;
           // std::cout << "Fps: " << int(1 / DeltaTime) << "\n";
        }

        keyInput();
        cameraMovement();

        View = cameraInst.getViewMatrix();



        obj.prepareRender();

        (obj.shaderProgram)->setMat4("projection", PROJECTION);

        (obj.shaderProgram)->setMat4("view", View);

        (obj.shaderProgram)->setMat4("model", obj.getModelMatrix());

        printVec3(glm::fvec1(DeltaTime * 100)* obj.forward());
       // obj.setPos((glm::fvec1(DeltaTime * 1)*- obj.forward())+ obj.getPos());

        obj.objUpdate();
        
        glm::vec3 diff = (obj.getPos() - cameraInst.getPos());
        /*if (glm::abs(diff.x) + glm::abs(diff.y) + glm::abs(diff.z) < 0.85f)
        {
            break;
        }*/
        glDrawArrays(GL_TRIANGLES, 0, obj.verticesNum/3);
        glBindVertexArray(0);

        obj2.prepareRender();

        (obj2.shaderProgram)->setMat4("projection", PROJECTION);

        (obj2.shaderProgram)->setMat4("view", View);

        (obj2.shaderProgram)->setMat4("model", obj2.getModelMatrix());



        glDrawArrays(GL_TRIANGLES, 0, obj2.verticesNum / 3);
        glBindVertexArray(0);

        obj3.prepareRender();

        (obj3.shaderProgram)->setMat4("projection", PROJECTION);

        (obj3.shaderProgram)->setMat4("view", View);

        (obj3.shaderProgram)->setMat4("model", obj3.getModelMatrix());



        glDrawArrays(GL_TRIANGLES, 0, obj3.verticesNum / 3);
        glBindVertexArray(0);
        
        obj4.prepareRender();

        (obj4.shaderProgram)->setMat4("projection", PROJECTION);

        (obj4.shaderProgram)->setMat4("view", View);

        (obj4.shaderProgram)->setMat4("model", obj4.getModelMatrix());



        glDrawArrays(GL_TRIANGLES, 0, obj4.verticesNum / 3);


        glBindVertexArray(0);



        glfwSwapBuffers(Window);
        glfwPollEvents();

    }


    glfwTerminate();
    return 0;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
