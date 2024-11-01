#pragma once

// Bibliotecas OpenGL e GLM
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

// Bibliotecas do projeto
#include "gameObject.h"
#include "main.h"
#include "shader.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "teapot.h"
#include "scene.h"
#include "material.h"

// Outras bibliotecas
#include <iostream>
/** TODO:
1. GameObject
    2. Create function pointers for `beforeUpdate` and `afterUpdate`.
    3. Implement EBO optimization.
    4. Load data from files:
        4.0 Constructor should accept a file path instead of vertices and vertex count.
        4.1 Method to load vertices from files.
        4.2 Method to load normals from files.
        4.3 The must have EBO at somepoint
    5. Add texture options, including repeat, mipmap settings, and others.
    6. Determine if some elements can be moved to smaller, more specialized classes. https://www.youtube.com/watch?v=IMZMLvIwa-k

2. Material
    Done for now;
3. Create a default config file move the size of the window there default elements and everthing this is done to be easy to be set , maybe create
**/


// Constantes de tempo e janela
Material DEFAULT_MATERIAL = Material();

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const double NEAR_FRUSTUM = 0.1f;
const double FAR_FRUSTUM = 1000000000;
const int AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 2000;
const glm::mat4 PROJECTION = glm::perspective(glm::radians(50.0f), (float)WIDTH / HEIGHT, (float)NEAR_FRUSTUM, (float)FAR_FRUSTUM);

// Variáveis de tempo
float DeltaTime = 0.0f;
float LastTime = 0.0f;

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


// Câmeras
Camera cameraInst = Camera(glm::vec3(5, 2, -3), true);

// Matrizes de visualização
glm::mat4 View;


int FpsSampleCounter = 0;
float DeltatimeMean = 0.0f;
void printVec3(const glm::vec3& vec) 
{
    std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << "\n";
}

void objUpdate(GameObject *obj)
{
    if (isKeyPressed(Window, GLFW_KEY_Z) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0.0f, 0.0f , Speed * 40.0f * DeltaTime));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Z))
    {
        obj->setScale(obj->getScale() + glm::vec3(0.0f, 0.0f, Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_X) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(Speed * 40.0f * DeltaTime, 0.0f , 0.0f));
    }
    else if (isKeyPressed(Window, GLFW_KEY_X))
    {
        obj->setScale(obj->getScale() + glm::vec3(Speed * DeltaTime, 0.0f, 0.0f));
    }

    if (isKeyPressed(Window, GLFW_KEY_Y) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        obj->setRot(obj->getRot() + glm::vec3(0.0f, Speed * 40.0f * DeltaTime, 0.0f));
    }
    else if (isKeyPressed(Window, GLFW_KEY_Y))
    {
        obj->setScale(obj->getScale() + glm::vec3(0.0f, Speed * DeltaTime, 0.0f));
    }

    if (isKeyPressed(Window, GLFW_KEY_UP))
    {
        obj->setPos(obj->getPos() + glm::vec3(0.0f, 0.0f, Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_DOWN))
    {
        obj->setPos(obj->getPos() + glm::vec3(0.0f, 0.0f, -Speed * DeltaTime));
    }

    if (isKeyPressed(Window, GLFW_KEY_RIGHT))
    {
        obj->setPos(obj->getPos() + glm::vec3(Speed * DeltaTime, 0.0f, 0.0f));
    }

    if (isKeyPressed(Window, GLFW_KEY_LEFT))
    {
        obj->setPos(obj->getPos() + glm::vec3(-Speed * DeltaTime, 0.0f, 0.0f));
    }

    
}

void keyInput()
{
    
    if (isKeyPressed(Window, GLFW_KEY_S))
    {
        glm::vec3 newPos = cameraInst.getPos() + glm::fvec1(-Speed * DeltaTime) * cameraInst.foward();
        cameraInst.setPos(newPos);
    }
    if (isKeyPressed(Window, GLFW_KEY_W))
    {
        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1(Speed * DeltaTime) * cameraInst.foward();
        cameraInst.setPos(newPos);
       
    }
    if (isKeyPressed(Window, GLFW_KEY_D))
    {

        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1(Speed * DeltaTime) * cameraInst.right();
        cameraInst.setPos(newPos);
    }
    if (isKeyPressed(Window, GLFW_KEY_A))
    {
        glm::vec3  newPos = cameraInst.getPos() + glm::fvec1(-Speed * DeltaTime) * cameraInst.right();
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
        Speed  = DefaultSpeed * 3.0f;
    }
    else if (Speed <= DefaultSpeed && isKeyPressed(Window, GLFW_KEY_LEFT_CONTROL))
    {
        Speed = DefaultSpeed * 0.25f;

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
    Shader LightShader("Shaders/vertexShaderDefault.glsl", "Shaders/fragmentShaderLight.glsl");
    DEFAULT_SHADER_REFERENCE = &DefaultShader;
    const unsigned int numVertices = sizeof(vertices) / sizeof(vertices[0]);


    #pragma region Lights
    Light l1 = Light(glm::vec3(25.0f, 100.0f, -25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.65f, 0.011f), false, 1000.0f);
    Light l2 = Light(glm::vec3(25.0f, 5.0f, -130.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.011f, 1.0f, 0.011f), false, 50.0f);
    Light l3 = Light(glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 1.0f), false, 1.0f);
    Light l4 = Light(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.8f, 0.6f), false, 1.0f);
    Light l5 = Light(glm::vec3(-130.0f, 200.0f, -100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.11f, 0.8f), false, 1600.0f);
    Light l6 = Light(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.3f), false, 1.9f);
    Light l7 = Light(glm::vec3(0.0f, 820.0f, 350.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), false, 15000.0f);
    Light l8 = Light(glm::vec3(-10.0f, 10.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.8f, 0.6f), false, 1.0f);
    Light l9 = Light(glm::vec3(25.0f, 5.0f, -105.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.011f, 0.011f, 1.0f), false, 30.3f);
    Light l10 = Light(glm::vec3(25.0f, 5.0f, -70.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.011f, 0.011f), false, 30.3f);
    Light* lights[10] = { &l1, &l2, &l3, &l4, &l5,&l6,&l7,&l8,&l9,&l10 };
    #pragma endregion

    #pragma region GameObjects
    //Material(const glm::vec4& colorV, const float& roughnessAmt, const float& amtOfSpecular)
    Material veryShine(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), 3, 30);

    GameObject obj = GameObject(glm::vec3(0.0f, 0.0f, -40.0f), glm::vec3(0.0f, 0.0f, 0.0f), vertices, numVertices, &DefaultShader, true);

    const char* pathss[] = { "TextureImages/wall.jpg" };
    obj.setTextures(texture, sizeof(texture), pathss, 1);
    obj.setScale(obj.getScale()* glm::fvec1(2));

    GameObject teapot = GameObject(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f), teapotVertices, teapot_count, DEFAULT_SHADER_REFERENCE, false);
    teapot.setUpdateFunc(&objUpdate);

    GameObject ground = GameObject(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), vertices, numVertices, &DefaultShader, true);
    ground.setScale(glm::vec3(500.0f, 1.0f, 500.0f));

    GameObject wall = GameObject(glm::vec3(0.0f, 248.5f, -250.0f), glm::vec3(0.0f, 0.0f, 0.0f), vertices, numVertices, &DefaultShader, true);
    wall.setScale(glm::vec3(500.0f, 500.0f, 1.0f));
    wall.setMaterial(&veryShine);

    GameObject* arr[14] = { &obj, &teapot, &ground, &wall };

    // Adicionando representação das luzes
    for (int i = 0; i < 10; i++) 
    {
        //lights[i]->getPos() 
        lights[i]->enablePhysicalRepresentation(vertices, numVertices, &LightShader);
        lights[i]->setScale(lights[i]->getScale()* glm::fvec1(lights[i]->getIntensity()/20));
        if (i == 0)
        {
           
            const char* paths[] = { "TextureImages/glowstone.jpg" };
            lights[i]->setTextures(texture, sizeof(texture), paths, 1);
        }
       
        //lights[i]->setTextures()
        arr[i + 4] = lights[i];
    }

    #pragma endregion
    float lightSpeed = -100.0f;
    Scene mainScene(arr, 14, lights, 10, &cameraInst, PROJECTION);

    while (!glfwWindowShouldClose(Window))
    {
        processInput(Window);

        glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;

        fpsLog();

        keyInput();
        cameraMovement();
        if (l1.getPos().z < -150 || l1.getPos().z > 50 )
        {
            lightSpeed *= -1;
        }
        l1.setPos(l1.getPos() + glm::vec3(0 , 0, DeltaTime * lightSpeed));

        mainScene.render();
        if (isKeyPressed(Window, GLFW_KEY_LEFT_CONTROL) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT) && isKeyPressed(Window, GLFW_KEY_T))
        {
            cameraInst.setPos(teapot.getPos() - glm::vec3(0, -5, -2.5));
            cameraInst.setTarget(teapot.getPos());

        }


        glfwSwapBuffers(Window);
        glfwPollEvents();

    }
    

    glfwTerminate();
   /* for (int i = 3; i < 13; i++)
    {
        delete arr[i];
    }*/
    return 0;
}

void fpsLog()
{
    DeltatimeMean = (DeltatimeMean * 0.2f + DeltaTime * 0.8f) / 2.0f;
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
