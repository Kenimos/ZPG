// Scene.h
#pragma once

// No OpenGL headers included here


#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Light.h"
#include "FlashLight.h"
#include <vector>
#include "Generator.h"

class Scene {
public:
    Scene(Camera* camera);
    ~Scene();

    void initShaders();
    void switchShaders();

    void initScene1();
    void initScene2();
    void initScene3();
    void initScene4();

    void draw();
    void update(float deltaTime);

    void toggleFlashLightIntensity();

private:
    Camera* camera;

    // Models
    Model* treeModel;
    Model* bushModel;
    Model* sphereModel;
    Model* giftModel;

    // Shaders
    ShaderProgram* constantShader;
    ShaderProgram* lambertShader;
    ShaderProgram* phongShader;
    ShaderProgram* blinnPhongShader;
    ShaderProgram* spotlightShader;

    std::vector<ShaderProgram*> shaders;
    int currentShaderIndex;

    // Drawable Objects
    std::vector<DrawableObject*> drawableObjects;

    // Lights
    std::vector<Light*> lights;
    FlashLight* flashLight;

    // For Scene 3: Indices of light spheres in drawableObjects
    std::vector<size_t> lightSphereIndices;

    // Random generator
    Generator generator;

    // Helper Methods
    void loadModelsScene1();
    void loadModelsScene2();
    void loadModelsScene3();
    void loadModelsScene4();
};
