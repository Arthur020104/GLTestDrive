
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
#include "light.h"
#include <iostream>
#include "teapot.h"
//Callback function for when window is resized
//Funcao de callback para quando a janela eh resized(redimensionada)



double DeltaTime = 0;
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

const double NEAR_FRUSTUM = 0.1f;
const double FAR_FRUSTUM = 1000000000;

GLFWwindow* Window;
Shader* DEFAULT_SHADER_REFERENCE;

unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
unsigned int Texture1, Texture2;

double LastTime = 0;

float Speed = 8.0f;
float DefaultSpeed = 8.0f;
float VerticalSens = 100.0f;
float HorizontalSens = 100.0f;

int const AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 20000;

glm::mat4 View;

Camera cameraInst = Camera(glm::vec3(5,2,-3), true);
Camera cameraInst2 = Camera(glm::vec3(0, 0, -50), glm::vec3(0, 0, 1), false);


const glm::mat4 PROJECTION = glm::perspective(glm::radians(103.0f), (float)WIDTH / HEIGHT, (float)NEAR_FRUSTUM, (float)FAR_FRUSTUM);

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
    Light lights[] = {
      Light(glm::vec3(25, 5, -25), glm::vec3(0, 0, 0), glm::vec3(1, 0.65, 0), false, 25.0f),       
     Light(glm::vec3(25, 5, -130), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), false, 50.0f),
    Light(glm::vec3(-10, 10, -10), glm::vec3(0, 0, 0), glm::vec3(0.8, 0.8, 1.0), false, 1.0f),  
    Light(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.8, 0.6), false, 1.0f),     
    Light(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.7, 0.5), false, 1.0f),    
    Light(glm::vec3(-10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.5, 0.3), false, 1.9f),  
    Light(glm::vec3(10, 10, -10), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.9, 0.7), false, 1.0f),    
    Light(glm::vec3(-10, 10, 5), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.8, 0.6), false, 1.0f),     
    Light(glm::vec3(25, 5, -105), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), false, 30.3f),
    Light(glm::vec3(25, 5, -70), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), false, 30.3f)
    };

    



    Shader DefaultShader("Shaders/vertexShaderDefault.glsl", "Shaders/fragmentShaderDefault.glsl");
    DEFAULT_SHADER_REFERENCE = &DefaultShader;
  //  (const glm::vec3& inicialPos, const glm::vec3& inicialRot, const glm::vec4& color, const float* vertices, unsigned long long int& sizeOfVertices, Shader* shaderProgramRef, const bool& isStatic) : TransformController(inicialPos, inicialRot)

    GameObject teapot = GameObject(glm::vec3(5, 0, 0),  glm::vec3(0, 0, 180), glm::vec4(1, 1,1,1), teapotVertices, teapot_count, DEFAULT_SHADER_REFERENCE, false);
    teapot.updateLighting(lights, 10);
    teapot.setUpdateFunc(&objUpdate);

    /*const char* paths12[] =
    {
    "TextureImages/scene.jpg",
    //"TextureImages/awesomeface.png"
    };

    teapot.setTextures(texture, sizeof(texture), paths12, 1);*/
    const int numVertices = sizeof(vertices) / sizeof(vertices[0]);
    GameObject obj = GameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), vertices, numVertices, DEFAULT_SHADER_REFERENCE, false);

    obj.updateLighting(lights, 10);

   // obj.setUpdateFunc(&objUpdate);

    const char* paths[] =
    {
    "TextureImages/scene.jpg",
    //"TextureImages/awesomeface.png"
    };
    obj.setTextures(texture, sizeof(texture), paths, 1);

    GameObject obj2 = GameObject(glm::vec3(2, 1, 0), glm::vec3(0, 0, 0), vertices, numVertices, DEFAULT_SHADER_REFERENCE, false);

    obj2.updateLighting(lights, 10);

    const char* paths2[] =
    {
    "TextureImages/container.jpg",
    };
    obj2.setTextures(texture, sizeof(texture), paths2, 1);

    GameObject obj3 = GameObject(glm::vec3(0, 10, -50), glm::vec3(0, 0, 0), vertices, numVertices, DEFAULT_SHADER_REFERENCE);
    obj3.updateLighting(lights, 10);

    const char* paths3[] =
    {
    "TextureImages/awesomeface.png",
    };
    obj3.setTextures(texture, sizeof(texture), paths3, 1);

    GameObject obj4 = GameObject(glm::vec3(2.0f, -0.5f, 0), glm::vec3(0, 0, 0), vertices, numVertices, DEFAULT_SHADER_REFERENCE);
    obj4.updateLighting(lights, 10);
    const char* paths4[] =
    {
    "TextureImages/awesomeface.pngs",
    "TextureImages/container.jpg",
    "TextureImages/wall.jpg"

    };
    obj4.setTextures(texture, sizeof(texture), paths4, 3);


    glEnable(GL_DEPTH_TEST);


    bool isOnDefaultCam = true;

    int FpsSampleCounter = 0;
    double DeltatimeMean = 0;


    // light teste2;
    std::vector<GameObject*> allObjects;
    allObjects.reserve(15);
    allObjects.push_back(&obj);
    allObjects.push_back(&obj2);
    allObjects.push_back(&obj3);
    allObjects.push_back(&obj4);
    allObjects.push_back(&teapot);

    
    //adicionando objetos para representarem as luzes
    for (int i = 0; i < 10; ++i)
    {
        Light& light = lights[i];

        glm::vec4 objectColor = glm::vec4(light.getColor()*glm::fvec1(light.getIntensity()*2), 1.0f);

        // Criando o GameObject na mesma posição da luz
        GameObject* object1 = new GameObject(light.getPos(), glm::vec3(0, 0, 0), objectColor, vertices, numVertices, DEFAULT_SHADER_REFERENCE, true);
        object1->setScale(object1->getScale() * glm::fvec1(light.getIntensity()*0.25));
        object1->updateLighting(lights, 10);
        allObjects.push_back(object1);
    }

    // Mean for every 2 frames with weights 0.8 and 0.2, with the newer frame being weighted higher.
    while (!glfwWindowShouldClose(Window))
    {
        processInput(Window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;



        DeltatimeMean = (DeltatimeMean * 0.2 + DeltaTime * 0.8) / 2;
        FpsSampleCounter++;
        if (FpsSampleCounter >= AMOUNT_OF_SAMPLES_TO_COUNT_FPS)
        {
            FpsSampleCounter = 0;
            std::cout << "Fps: " << int(1 / DeltaTime) << "\n";
        }

        keyInput();
        cameraMovement();
        if (isKeyPressed(Window, GLFW_KEY_C) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
        {
            isOnDefaultCam = true;
        }
        else if (isKeyPressed(Window, GLFW_KEY_C))
        {
            isOnDefaultCam = false;
        }

 /*       if (isKeyPressed(Window, GLFW_KEY_1))
        {
            teste.position.y += DeltaTime * Speed / 2;
        }
        else if (isKeyPressed(Window, GLFW_KEY_0))
        {
            teste.position.y += DeltaTime * -Speed / 2;
        }
        */
        if (isOnDefaultCam)
        {
            View = cameraInst.getViewMatrix();
        }
        else
        {
            View = cameraInst2.getViewMatrix();
        }

        for (GameObject* objeto : allObjects)
        {
            objeto->objUpdate();
            
            objeto->prepareRender(View, PROJECTION);

            glDrawArrays(GL_TRIANGLES, 0, objeto->verticesNum / 3);
            glBindVertexArray(0);
        }



        glfwSwapBuffers(Window);
        glfwPollEvents();

    }
    for (int i =5;i<15;i++)
    {
        delete(allObjects[i]);
    }

    glfwTerminate();
    return 0;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
