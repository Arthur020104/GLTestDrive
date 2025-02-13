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







/*
* Focus points will be highlighted, for example this is the third function on the file and the second PROBLEM on this function
* Named: PROBLEM 3.2 [DESCRIPTION]
*/












// Constantes de tempo e janela
Material DEFAULT_MATERIAL;

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const double NEAR_FRUSTUM = 0.1f;
const double FAR_FRUSTUM = 1000000000;
const int AMOUNT_OF_SAMPLES_TO_COUNT_FPS = 20000;
const glm::mat4 PROJECTION = glm::perspective(glm::radians(55.0f), (float)WIDTH / HEIGHT, (float)NEAR_FRUSTUM, (float)FAR_FRUSTUM);

// Variáveis de tempo
float DeltaTime = 0.0f;
float LastTime = 0.0f;

// Variáveis de controle de posição do mouse
double LastXPos = -100000, LastYPos = -100000;

// Variáveis de sensibilidade de movimento
float Speed = 8.0f;
float DefaultSpeed = 18.0f;
float VerticalSens = 100.0f;
float HorizontalSens = 100.0f;
float IncreaseIntensitySpeed = 30.0f;
int aspectOfLightBeingChanged = 0;
bool wait = false;

// Janela e shaders
GLFWwindow* Window;
Shader* DEFAULT_SHADER_REFERENCE;


// Câmeras
Camera cameraInst = Camera(glm::vec3(5, 2, -3), true);

// Matrizes de visualização
glm::mat4 View;
int whereToChangeColor = 0;
bool adding = true;
void dinamicLightUpdate(GameObject* obj)
{

    float rate = 1.0f;  
    
    Light* l = (Light*)obj;
    glm::vec3 addToColor = l->getColor();

    addToColor[whereToChangeColor] += rate * DeltaTime * (adding ? 1 : -1);

  
    if (addToColor[whereToChangeColor] >= 1.0f)
    {
        addToColor[whereToChangeColor] = 1.0f;
        adding = false;
    }
    else if (addToColor[whereToChangeColor] <= 0.1f)
    {
        addToColor[whereToChangeColor] = 0.1f;
        adding = true;
    }
    if (addToColor[whereToChangeColor] == 1.0f || addToColor[whereToChangeColor] == 0.1f)
    {
        whereToChangeColor++;
        if (whereToChangeColor >= 3)
        {
            whereToChangeColor = 0;
        }
    }

    l->setColor(addToColor);

}

void followCamera(GameObject* obj)
{
    obj->setPos(cameraInst.getPos() - cameraInst.foward());
    Light* l = dynamic_cast<Light*>(obj);
    l->setDirection(cameraInst.foward());
    glm::vec3 up = glm::cross(cameraInst.foward(), cameraInst.right());

    
    double yaw = glm::atan(cameraInst.foward().x, cameraInst.foward().z);

    double pitch = glm::asin(cameraInst.foward().y);

    double roll = glm::atan(up.y, up.z);

    
    //obj->setRot(glm::vec3(pitch, yaw, roll));

}

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
        Speed  = DefaultSpeed * 8.0f;
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
void l9Func(GameObject* obj)
{
    
    if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        
        obj->setPos(obj->getPos() + glm::fvec1(Speed * DeltaTime) * cameraInst.right());
    }
    else if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        obj->setPos(obj->getPos() + glm::fvec1(-Speed * DeltaTime) * cameraInst.right());
    }
    if (scrolling && scrollingUp)
    {
        
        obj->setPos(obj->getPos() + glm::fvec1(Speed *10 * DeltaTime) * glm::vec3(0.0f,1.0f,0.0f));
    }
    else if (scrolling)
    {
        obj->setPos(obj->getPos() + glm::fvec1(-Speed * 10 * DeltaTime) * glm::vec3(0.0f, 1.0f, 0.0f));
    }
    





    if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT) && isKeyPressed(Window, GLFW_KEY_EQUAL))
    {
        if (wait)
        {
            return;
        }
        aspectOfLightBeingChanged++;
        if (aspectOfLightBeingChanged > 3)
            aspectOfLightBeingChanged = 0;
        std::cout << aspectOfLightBeingChanged << "\n";
        wait = true;

    }
    else if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT) && isKeyPressed(Window, GLFW_KEY_MINUS))
    {
        if (wait)
        {
            return;
        }
        aspectOfLightBeingChanged--;
        if (aspectOfLightBeingChanged < 0)
            aspectOfLightBeingChanged = 3;
        std::cout << aspectOfLightBeingChanged << "\n";
        wait = true;
    }
    else
    {
        wait = false;
    }





    if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_MINUS) && !isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        Light* l = dynamic_cast<Light*>(obj);
        if (l == nullptr)
        {
            std::cerr << "Could not cast to Light pointer, returning\n";
            return;
        }
        switch (aspectOfLightBeingChanged)
        {
            case 0:
                l->setIntensity(l->getIntensity() - DeltaTime * IncreaseIntensitySpeed);
                l->setScale(l->getScale() + glm::vec3(-1) * DeltaTime * (IncreaseIntensitySpeed / 100));
                break;
            case 1:
                l->setAmbient(l->getAmbient() + glm::vec3(-1) * DeltaTime * IncreaseIntensitySpeed);
                break;
            case 2:
                l->setDiffuse(l->getDiffuse() + glm::vec3(-1) * DeltaTime * IncreaseIntensitySpeed);
                break;
            case 3:
                l->setSpecular(l->getSpecular() + glm::vec3(-1) * DeltaTime * IncreaseIntensitySpeed);
                break;
        }
    }       
    else if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_EQUAL) && !isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        Light* l = dynamic_cast<Light*>(obj);
        if (l == nullptr)
        {
            std::cerr << "Could not cast to Light pointer, returning\n";
            return;
        }
        switch (aspectOfLightBeingChanged)
        {
            case 0:
                l->setIntensity(l->getIntensity() + DeltaTime * IncreaseIntensitySpeed);
                l->setScale(l->getScale() + glm::vec3(1) * DeltaTime * (IncreaseIntensitySpeed / 100));
                break;
            case 1:
                l->setAmbient(l->getAmbient() + glm::vec3(1) * DeltaTime * IncreaseIntensitySpeed);
                break;
            case 2:
                l->setDiffuse(l->getDiffuse() + glm::vec3(1) * DeltaTime * IncreaseIntensitySpeed);
                break;
            case 3:
                l->setSpecular(l->getSpecular() + glm::vec3(1) * DeltaTime * IncreaseIntensitySpeed);
                break;
        }
    }
    /*
    if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_MINUS) && isKeyPressed(Window,GLFW_KEY_LEFT_SHIFT))
    {
        Light* l = dynamic_cast<Light*>(obj);
        if (l == nullptr)
        {
            std::cerr << "Could not cast to Light pointer, returning\n";
            return;
        }
        l->setAmbient(l->getAmbient() + glm::vec3(-1) * DeltaTime * IncreaseIntensitySpeed);
    }
    else if (isKeyPressed(Window, GLFW_KEY_9) && isKeyPressed(Window, GLFW_KEY_EQUAL) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT))
    {
        Light* l = dynamic_cast<Light*>(obj);
        if (l == nullptr)
        {
            std::cerr << "Could not cast to Light pointer, returning\n";
            return;
        }
        l->setAmbient(l->getAmbient() + glm::vec3(1) * DeltaTime * IncreaseIntensitySpeed);
    }*/

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

void objAftherUpdate(GameObject* obj) 
{
    if (isKeyPressed(Window, GLFW_KEY_LEFT_CONTROL) && isKeyPressed(Window, GLFW_KEY_LEFT_SHIFT) && isKeyPressed(Window, GLFW_KEY_T))
    {
        cameraInst.setPos(obj->getPos() - glm::vec3(0, -1, -8.5));
        cameraInst.setTarget(obj->getPos());

    }
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
    glfwSetScrollCallback(*window, scrollCallback);

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
Scene* prepareScene() 
{
    // Vertex data for a cube
    float vertices[] = {
        // Positions for 36 vertices (6 faces of 2 triangles each)
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
    const unsigned int numVertices = sizeof(vertices) / sizeof(vertices[0]);

    // Create shaders
    Shader* defaultShader = new Shader("Shaders/vertexShaderDefault.glsl", "Shaders/fragmentShaderDefault.glsl");
    Shader* lightShader = new Shader("Shaders/vertexShaderDefault.glsl", "Shaders/fragmentShaderLight.glsl");
    DEFAULT_SHADER_REFERENCE = defaultShader; // Assuming this global is used elsewhere

    // Dynamically allocate only the required lights (light 7, 9, and 10)
    Light* l7 = new Light
    (
        glm::vec3(0.0f, 820.0f, 350.0f),
        glm::vec3(0.0f),
        glm::vec3(0.6f, 0.5f, 0.5f),
        false, 4.0f, true
    );
    Light* l9 = new Light
    (
        glm::vec3(10.0f, 3.0f, -35.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        false, 0.0f, 12.5f
    );
    Light* l10 = new Light
    (
        glm::vec3(10000.0f, 10000.0f, 10000.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        true, 0.1f
    );
    l9->setUpdateFunc(&l9Func);
    l7->setScale(glm::vec3(1.0f));
    l7->setAftherUpdateFunc(&followCamera);

    // Set up materials
    const char* boxDiffusePath = "C:/Users/arthu/Downloads/container2.png";
    const char* boxSpecularPath = "C:/Users/arthu/Downloads/container2_specular.png";
 

    Material* boxMat = new Material(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 50);
    boxMat->LoadDiffuseMap(boxDiffusePath);
    boxMat->LoadSpecularMap(boxSpecularPath);


    // Create a vector to hold all scene objects (GameObjects and Lights)
    std::vector<GameObject*> sceneObjects;
   // sceneObjects.reserve(6 + 2 + 3 + 2); // Reserve for 6 boxes, 2 static objects, and 3 lights

    // Create 6 box objects in a circular layout
    constexpr float DEG2RAD = glm::pi<float>() / 180.0f;
    float angleIncrement = 60.0f;
    for (int i = 0; i < 6; ++i) 
    {
        float angle = i * angleIncrement * DEG2RAD;
        glm::vec3 position = glm::vec3(5.0f * glm::cos(angle), 1.0f, 5.0f * glm::sin(angle));
        glm::vec3 rotation = glm::vec3(0.0f, angle * 30.0f, 0.0f);
        GameObject* box = new GameObject(position, rotation, vertices, numVertices, defaultShader, true);
        box->setMaterial(boxMat);
        box->setUpdateFunc(&objUpdate);
        box->setAftherUpdateFunc(&objAftherUpdate);
        sceneObjects.push_back(box);
    }

    // Create additional static objects: obj2 and ground
    GameObject* obj2 = new GameObject
    (
        glm::vec3(-15.0f, 5.0f, -40.0f),
        glm::vec3(0.0f),
        vertices,
        numVertices,
        defaultShader,
        true
    );
    obj2->setMaterial(boxMat);  // Using the same material for simplicity
    sceneObjects.push_back(obj2);



    // Add the dynamically allocated lights to the scene
    sceneObjects.push_back(l7);
    sceneObjects.push_back(l9);
    sceneObjects.push_back(l10);

    /*House*/

    //Material* boxMat = new Material(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 50);
    //
    const char* woodDiffusePath = "C:/Users/arthu/Downloads/1ky60nq9nh091.png";
    Material* woodMat = new Material(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 10);
    woodMat->setSpecularMulti(0.3f);
    woodMat->setRepeatTexture(true);
    woodMat->setRepeatTextureFactor(glm::vec3(0.3,0.3,1));
    woodMat->LoadDiffuseMap(woodDiffusePath);
    const float multiFHouse = 65.0f;

    GameObject* wall1 = new GameObject(glm::vec3(multiFHouse / 2, multiFHouse / 4,0),glm::vec3(0,90,0), vertices, numVertices, defaultShader, true);
    wall1->setScale(wall1->getScale() * glm::vec3(multiFHouse, multiFHouse/2,1.0f));
    GameObject* wall2 = new GameObject(glm::vec3(-multiFHouse / 2, multiFHouse / 4, 0), glm::vec3(0, 90, 0), vertices, numVertices, defaultShader, true);
    wall2->setScale(wall2->getScale() * glm::vec3(multiFHouse, multiFHouse/2, 1.0f));
    GameObject* wall3 = new GameObject(glm::vec3(0, multiFHouse / 4, multiFHouse/2), glm::vec3(0, 0, 0), vertices, numVertices, defaultShader, true);
    wall3->setScale(wall3->getScale() * glm::vec3(multiFHouse, multiFHouse / 2, 1.0f));
    GameObject* wall4 = new GameObject(glm::vec3(0, multiFHouse / 4, -multiFHouse / 2), glm::vec3(0, 0, 0), vertices, numVertices, defaultShader, true);
    wall4->setScale(wall4->getScale() * glm::vec3(multiFHouse, multiFHouse / 2, 1.0f));
    GameObject* wall5 = new GameObject(glm::vec3(0, 0,0), glm::vec3(90, 0, 0), vertices, numVertices, defaultShader, true);
    wall5->setScale(wall5->getScale() * glm::vec3(multiFHouse, multiFHouse, 1.0f));
    GameObject* wall6 = new GameObject(glm::vec3(0, multiFHouse/2, 0), glm::vec3(90, 0, 0), vertices, numVertices, defaultShader, true);
    wall6->setScale(wall6->getScale() * glm::vec3(multiFHouse, multiFHouse, 1.0f));
    wall1->setMaterial(woodMat);
    wall2->setMaterial(woodMat);
    wall3->setMaterial(woodMat);
    wall4->setMaterial(woodMat);
    wall5->setMaterial(woodMat);
    wall6->setMaterial(woodMat);


    sceneObjects.push_back(wall1);
    sceneObjects.push_back(wall2);
    sceneObjects.push_back(wall3);
    sceneObjects.push_back(wall4);
    sceneObjects.push_back(wall5);
    sceneObjects.push_back(wall6);
     /*House*/


    // Create the scene. Assumes that 'cameraInst' and 'PROJECTION' are defined elsewhere.
    Scene* mainScene = new Scene(sceneObjects.data(), sceneObjects.size(), &cameraInst, PROJECTION);
    return mainScene;
}

int main()
{

    if (init(&Window) == -1)
    {
        return -1;
    }
    
    float lightSpeed = -55.0f;
    Scene* main = prepareScene();


    while (!glfwWindowShouldClose(Window))
    {
       
       
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();
        DeltaTime = timeValue - LastTime;
        LastTime = timeValue;
            

        fpsLog();


        keyInput();
        cameraMovement();
        

        main->render();
        processInput(Window);


        glfwSwapBuffers(Window);
        glfwPollEvents();

    }

    glfwTerminate();
  
    return 0;
}

void fpsLog()
{
    DeltatimeMean = (DeltatimeMean * 0.9f + DeltaTime * 0.1f) / 2.0f;
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
