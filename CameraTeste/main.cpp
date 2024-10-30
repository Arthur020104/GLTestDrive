#pragma once

// Bibliotecas OpenGL e GLM
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Bibliotecas do projeto
#include "gameObject.h"
#include "main.h"
#include "shader.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "teapot.h"
#include "scene.h"

// Outras bibliotecas
#include <iostream>
#include <memory>

// Constantes de tempo e janela
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const double NEAR_FRUSTUM = 0.1f;
const double FAR_FRUSTUM = 1000000000;
const int AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 20000;
const glm::mat4 PROJECTION = glm::perspective(glm::radians(103.0f), (float)WIDTH / HEIGHT, (float)NEAR_FRUSTUM, (float)FAR_FRUSTUM);

// Variáveis de tempo
double DeltaTime = 0;
double LastTime = 0;

// Variáveis de controle de posição do mouse
double LastXPos = -100000, LastYPos = -100000;

// Variáveis de sensibilidade de movimento
float Speed = 8.0f;
float DefaultSpeed = 8.0f;
float VerticalSens = 100.0f;
float HorizontalSens = 100.0f;

// Janela e shaders
GLFWwindow* Window;
Shader* DEFAULT_SHADER_REFERENCE;

// Buffers e texturas
unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
unsigned int Texture1, Texture2;

// Câmeras
Camera cameraInst = Camera(glm::vec3(5, 2, -3), true);

// Matrizes de visualização
glm::mat4 View;


int FpsSampleCounter = 0;
double DeltatimeMean = 0;
void printVec3(const glm::vec3& vec) 
{
    std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << "\n";
}

void objUpdate(GameObject *obj)
{
    if (isKeyPressed(Window, GLFW_KEY_Z) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0, 0, Speed * 40 * DeltaTime));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Z))
    {
        obj->setScale(obj->getScale() + glm::vec3(0, 0, Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_X) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(Speed * 40 * DeltaTime, 0, 0));
    }
    else if (isKeyPressed(Window, GLFW_KEY_X))
    {
        obj->setScale(obj->getScale() + glm::vec3(Speed * DeltaTime, 0, 0));
    }

    if (isKeyPressed(Window, GLFW_KEY_Y) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0, Speed * 40 * DeltaTime, 0));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Y))
    {
        obj->setScale(obj->getScale() + glm::vec3(0, Speed * DeltaTime, 0));
    }
    
    if (isKeyPressed(Window, GLFW_KEY_UP))
    {
        obj->setPos(obj->getPos() + glm::vec3(0,0, Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_DOWN))
    {
        obj->setPos(obj->getPos() + glm::vec3(0, 0, -Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_RIGHT))
    {
        obj->setPos(obj->getPos() + glm::vec3(Speed * DeltaTime, 0, 0));
    }

    if (isKeyPressed(Window, GLFW_KEY_LEFT))
    {
        obj->setPos(obj->getPos() + glm::vec3(-Speed * DeltaTime, 0, 0));
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
        Speed  = DefaultSpeed*10;
    }
    else if (Speed <= DefaultSpeed && isKeyPressed(Window, GLFW_KEY_LEFT_CONTROL))
    {
        Speed = DefaultSpeed * 0.25;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
    glEnable(GL_DEPTH_TEST);
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
    const int numVertices = sizeof(vertices) / sizeof(vertices[0]);


    #pragma region Lights
    Light l1 = Light(glm::vec3(25, 5, -25), glm::vec3(0, 0, 0), glm::vec3(1, 0.65, 0.011), false, 25.0f);
    Light l2 = Light(glm::vec3(25, 5, -130), glm::vec3(0, 0, 0), glm::vec3(0.011, 1, 0.011), false, 50.0f);
    Light l3 = Light(glm::vec3(-10, 10, -10), glm::vec3(0, 0, 0), glm::vec3(0.8, 0.8, 1.0), false, 1.0f);
    Light l4 = Light(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.8, 0.6), false, 1.0f);
    Light l5 = Light(glm::vec3(-130, 5, -100), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.11, 0.8), false, 590.0f);
    Light l6 = Light(glm::vec3(-10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.5, 0.3), false, 1.9f);
    Light l7 = Light(glm::vec3(0, 820, -50), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.9, 0.7), false, 000.0f);
    Light l8 = Light(glm::vec3(-10, 10, 5), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.8, 0.6), false, 1.0f);
    Light l9 = Light(glm::vec3(25, 5, -105), glm::vec3(0, 0, 0), glm::vec3(0.011, 0.011, 1), false, 30.3f);
    Light l10 = Light(glm::vec3(25, 5, -70), glm::vec3(0, 0, 0), glm::vec3(1, 0.011, 0.011), false, 30.3f);
    Light* lights[10] = { &l1, &l2, &l3, &l4, &l5,&l6,&l7,&l8,&l9,&l10 };
    #pragma endregion

    #pragma region GameObjects
    GameObject obj = GameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), vertices, numVertices, &DefaultShader, true);

    const char* paths[] = { "TextureImages/scene.jpg" };
    obj.setTextures(texture, sizeof(texture) , paths, 1);

    GameObject teapot = GameObject(glm::vec3(5, 0, 0), glm::vec3(0, 0, 180), teapotVertices, teapot_count, DEFAULT_SHADER_REFERENCE, false);
    teapot.setUpdateFunc(&objUpdate);

    GameObject ground = GameObject(glm::vec3(0, -1.5, 0), glm::vec3(0, 0, 0), vertices, numVertices, &DefaultShader, true);
    ground.setScale(glm::vec3(500, 1, 500));


    GameObject* arr[3] = {&obj, &teapot, &ground};
    #pragma endregion

    Scene mainScene(arr, 3, lights, 10, &cameraInst, PROJECTION);

    while (!glfwWindowShouldClose(Window))
    {
        processInput(Window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;

        fpsLog();

        keyInput();
        cameraMovement();


        mainScene.render();



        glfwSwapBuffers(Window);
        glfwPollEvents();

    }
    

    glfwTerminate();
    return 0;
}

void fpsLog()
{
    DeltatimeMean = (DeltatimeMean * 0.2 + DeltaTime * 0.8) / 2;
    FpsSampleCounter++;
    if (FpsSampleCounter >= AMOUNT_OF_SAMPLES_TO_COUNT_FPS)
    {
        FpsSampleCounter = 0;
        std::cout << "Fps: " << int(1 / DeltaTime) << "\n";
    }
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
