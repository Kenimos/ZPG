

#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include "Camera.h"
#include "Scene.h"
#include "ShaderProgram.h"

class App
{
public:
    App();
    ~App();

    bool init();
    void run();

private:
    GLFWwindow *window;
    Camera camera;
    ShaderProgram *shaderProgram;
    std::vector<Scene *> scenes;
    int width, height;
    int currentSceneIndex;

    std::array<bool, 10> keyStates;
    bool cameraControlActive;

    bool initGLFW();
    bool initGLEW();

    void processInput();
    void renderScene();

    std::string loadShaderSource(const std::string &filePath);

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
};

#endif
