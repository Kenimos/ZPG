
#include "App.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ShaderLoader.h"
#include "Scene.h"

App::App()
    : window(nullptr),
      camera(glm::vec3(0.0f, 0.0f, 5.0f)),
      width(800), height(600),
      currentSceneIndex(0),
      keyStates{},
      cameraControlActive(false)
{

}

App::~App()
{
    for (Scene *scene : scenes)
    {
        delete scene;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool App::init()
{
    if (!initGLFW())
    {
        return false;
    }

    window = glfwCreateWindow(width, height, "OpenGL Application", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "ERROR: Could not create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (!initGLEW())
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    Scene *scene1 = new Scene(&camera);

    scene1->initScene1();
    scenes.push_back(scene1);

    Scene *scene2 = new Scene(&camera);

    scene2->initScene2();
    scenes.push_back(scene2);

    Scene *scene3 = new Scene(&camera);

    scene3->initScene3();
    scenes.push_back(scene3);

    Scene *scene4 = new Scene(&camera);

    scene4->initScene4();
    scenes.push_back(scene4);

    currentSceneIndex = 0;

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowUserPointer(window, this);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return true;
}

void App::run()
{
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size())
        {
            scenes[currentSceneIndex]->update(deltaTime);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

bool App::initGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "ERROR: Could not start GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

bool App::initGLEW()
{
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: "
                  << glewGetErrorString(err) << std::endl;
        return false;
    }
    return true;
}

void App::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!cameraControlActive)
        {
            cameraControlActive = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.setFirstMouse(true); 
        }
    }
    else
    {
        if (cameraControlActive)
        {
            cameraControlActive = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    float currentFrame = glfwGetTime();
    static float lastFrame = 0.0f;
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.processKeyboard(window, deltaTime);

    static bool xKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        if (!xKeyPressed)
        {
            if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size())
            {
                scenes[currentSceneIndex]->switchShaders();
                std::cout << "Switched shaders in scene " << (currentSceneIndex + 1) << std::endl;
            }
            xKeyPressed = true;
        }
    }
    else
    {
        xKeyPressed = false;
    }

    for (int i = GLFW_KEY_1; i <= GLFW_KEY_9; ++i)
    {
        if (glfwGetKey(window, i) == GLFW_PRESS)
        {
            int sceneIndex = i - GLFW_KEY_1;
            if (!keyStates[sceneIndex])
            {
                if (sceneIndex < scenes.size())
                {

                    currentSceneIndex = sceneIndex;
                    std::cout << "Switched to scene " << (sceneIndex + 1) << std::endl;
                }
                keyStates[sceneIndex] = true;
            }
        }
        else if (glfwGetKey(window, i) == GLFW_RELEASE)
        {
            int sceneIndex = i - GLFW_KEY_1;
            keyStates[sceneIndex] = false;
        }
    }
    static bool fKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!fKeyPressed)
        {
            if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size())
            {
                scenes[currentSceneIndex]->toggleFlashLightIntensity();
            }
            fKeyPressed = true;
        }
    }
    else
    {
        fKeyPressed = false;
    }
}

void App::renderScene()
{
    if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size())
    {
        scenes[currentSceneIndex]->draw();
    }
}

void App::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->width = width;
    app->height = height;
    glViewport(0, 0, width, height);
    app->camera.setAspectRatio(static_cast<float>(width) / height);
}

void App::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app->cameraControlActive)
    {
        app->camera.processMouseMovement(xpos, ypos);
    }
}

void App::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->camera.processMouseScroll(yoffset);
}
