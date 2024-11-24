#pragma once



#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Light.h"
#include "FlashLight.h"
#include <vector>
#include "Generator.h"
#include "SkyBox.h"

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

    void initSkybox();
    void drawSkybox();
    void toggleSkyboxFollowing();


private:
    Camera* camera;

    Model* treeModel;
    Model* bushModel;
    Model* sphereModel;
    Model* giftModel;
    Model* rectangleModel;

    ShaderProgram* constantShader;
    ShaderProgram* lambertShader;
    ShaderProgram* phongShader;
    ShaderProgram* blinnPhongShader;
    ShaderProgram* spotlightShader;
    ShaderProgram* groundShader;

    std::vector<ShaderProgram*> shaders;
    int currentShaderIndex;

    std::vector<DrawableObject*> drawableObjects;

    std::vector<Light*> lights;
    FlashLight* flashLight;

    std::vector<size_t> lightSphereIndices;

    Generator generator;
    SkyBox* skybox;
    bool skyboxFollowsCamera; 

    void loadModelsScene1();
    void loadModelsScene2();
    void loadModelsScene3();
    void loadModelsScene4();
};
