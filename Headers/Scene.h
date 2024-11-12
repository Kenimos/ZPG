// Scene.h

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "DrawableObject.h"
#include "Camera.h"
#include "Generator.h"
#include "Light.h"
#include "FlashLight.h"

// Scene.h

class Scene
{
public:
    Scene(Camera *camera);
    ~Scene();

    void initScene1();
    void initScene2();
    void initScene3();
    void initScene4();

    void draw();
    void update(float deltaTime);

    void switchShaders();
    void toggleFlashLightIntensity();

private:
    Camera *camera;

    std::vector<DrawableObject *> drawableObjects;
    std::vector<Light *> lights;
    std::vector<ShaderProgram *> shaders;
    std::vector<size_t> lightSphereIndices;

    Model *treeModel;
    Model *bushModel;
    Model *sphereModel;
    Model *giftModel;

    ShaderProgram *constantShader;
    ShaderProgram *lambertShader;
    ShaderProgram *phongShader;
    ShaderProgram *blinnPhongShader;
    ShaderProgram *spotlightShader;

    int currentShaderIndex;

    void initShaders();
    void loadModelsScene1();
    void loadModelsScene2();
    void loadModelsScene3();
    void loadModelsScene4();

    FlashLight *flashLight;

    Generator generator;
};

#endif // SCENE_H
