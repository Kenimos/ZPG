#include <GL/glew.h>
#include "Scene.h"
#include "../Models/tree.h"
#include "../Models/bushes.h"
#include "../Models/sphere.h"
#include "../Models/gift.h"
#include <random>
#include <glm/glm.hpp>
#include <iostream>
#include "ShaderLoader.h"
#include "Transformation.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"
#include <glm/gtc/random.hpp>
#include <glm/ext/matrix_transform.hpp>

Scene::Scene(Camera *camera)
    : camera(camera),
      treeModel(nullptr), bushModel(nullptr), sphereModel(nullptr), giftModel(nullptr), rectangleModel(nullptr),
      constantShader(nullptr), lambertShader(nullptr), phongShader(nullptr), blinnPhongShader(nullptr),
      spotlightShader(nullptr), currentShaderIndex(0), flashLight(nullptr), skybox(nullptr), skyboxFollowsCamera(true)
{
}

Scene::~Scene()
{
    delete treeModel;
    delete bushModel;
    delete sphereModel;
    delete giftModel;
    delete rectangleModel;
    delete constantShader;
    delete lambertShader;
    delete phongShader;
    delete blinnPhongShader;
    delete spotlightShader;

    for (auto *object : drawableObjects)
    {
        delete object;
    }
    drawableObjects.clear();

    for (auto *light : lights)
    {
        delete light;
    }
    lights.clear();
}

void Scene::initShaders()
{
    ShaderLoader shaderLoader;

    constantShader = new ShaderProgram();
    GLuint constantProgramID = shaderLoader.loadShader("Shaders/constant_vertex.glsl", "Shaders/constant_fragment.glsl");
    constantShader->setProgramID(constantProgramID);

    lambertShader = new ShaderProgram();
    GLuint lambertProgramID = shaderLoader.loadShader("Shaders/lambert_vertex.glsl", "Shaders/lambert_fragment.glsl");
    lambertShader->setProgramID(lambertProgramID);

    phongShader = new ShaderProgram();
    GLuint phongProgramID = shaderLoader.loadShader("Shaders/phong_vertex.glsl", "Shaders/phong_fragment.glsl");
    phongShader->setProgramID(phongProgramID);

    blinnPhongShader = new ShaderProgram();
    GLuint blinnProgramID = shaderLoader.loadShader("Shaders/blinn_vertex.glsl", "Shaders/blinn_fragment.glsl");
    blinnPhongShader->setProgramID(blinnProgramID);

    spotlightShader = new ShaderProgram();
    GLuint spotlightProgramID = shaderLoader.loadShader("Shaders/spotlight_vertex.glsl", "Shaders/spotlight_fragment.glsl");
    spotlightShader->setProgramID(spotlightProgramID);

    shaders.push_back(constantShader);
    shaders.push_back(lambertShader);
    shaders.push_back(phongShader);
    shaders.push_back(blinnPhongShader);
    shaders.push_back(spotlightShader);

    for (auto *shader : shaders)
    {
        camera->attach(shader);
    }
    camera->notifyObservers();

    std::vector<std::string> faces{
        "/Users/jan/Desktop/ZPG/Textures/posx.jpg",
        "/Users/jan/Desktop/ZPG/Textures/negx.jpg",
        "/Users/jan/Desktop/ZPG/Textures/posy.jpg",
        "/Users/jan/Desktop/ZPG/Textures/negy.jpg",
        "/Users/jan/Desktop/ZPG/Textures/posz.jpg",
        "/Users/jan/Desktop/ZPG/Textures/negz.jpg"};
    skybox = new SkyBox(faces);
}

void Scene::switchShaders()
{
    currentShaderIndex = (currentShaderIndex + 1) % shaders.size();
    ShaderProgram *newShader = shaders[currentShaderIndex];
    for (auto *object : drawableObjects)
    {
        object->setShaderProgram(newShader);
    }
    std::cout << "Switched to shader index " << currentShaderIndex << std::endl;
}

void Scene::initScene1()
{
    initShaders();
    loadModelsScene1();
}

void Scene::initScene2()
{
    initShaders();
    loadModelsScene2();
}

void Scene::initScene3()
{
    initShaders();
    loadModelsScene3();
}

void Scene::initScene4()
{
    initShaders();
    loadModelsScene4();
}

void Scene::loadModelsScene1()
{
    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    Model *nickModel = new Model();
    nickModel->loadModel("Models/jec0014.obj");
    DrawableObject *nickObject = new DrawableObject(nickModel, phongShader);
    nickObject->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    nickObject->getTransformation().addComponent(std::make_shared<Scaling>(glm::vec3(1.0f)));
    nickObject->getTransformation().addComponent(std::make_shared<Translation>(glm::vec3(0.0f, 30.0f, 100.0f)));
    nickObject->getTransformation().addComponent(std::make_shared<Rotation>(180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    nickObject->getTransformation().addComponent(std::make_shared<Rotation>(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    drawableObjects.push_back(nickObject);


    Model *houseModel = new Model();
    houseModel->loadModel("Models/Cottage_FREE.obj");
    DrawableObject *houseObject = new DrawableObject(houseModel, phongShader);
    houseObject->setColor(glm::vec3(0.8f, 0.8f, 0.8f));
    houseObject->getTransformation().addComponent(std::make_shared<Scaling>(glm::vec3(1.0f)));
    houseObject->getTransformation().addComponent(std::make_shared<Translation>(glm::vec3(0.0f, 0.0f, 0.0f)));
    Texture *houseTexture = new Texture("Textures/Cottage_Clean_Base_Color.png");
    houseObject->setTexture(houseTexture);
    drawableObjects.push_back(houseObject);

    Model *frogModel = new Model();
    frogModel->loadModel("Models/frog.obj");
    Texture *frogTexture = new Texture("Textures/frog.jpg");
    for (int i = 0; i < 16; i++)
    {
        float f = i * 10.0f;
        DrawableObject *frogObject = new DrawableObject(frogModel, phongShader);
        frogObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        frogObject->getTransformation().addComponent(std::make_shared<Scaling>(glm::vec3(0.5f)));
        
        if (i > 7)
        {
            frogObject->getTransformation().addComponent(std::make_shared<Translation>(glm::vec3(-115.0f + f, 0.0f, -40.0f)));
            frogObject->getTransformation().addComponent(std::make_shared<Rotation>(270.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
            frogObject->getTransformation().addComponent(std::make_shared<Rotation>(180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
        }
        else
        {
            frogObject->getTransformation().addComponent(std::make_shared<Translation>(glm::vec3(-35.0f + f, 0.0f, 40.0f)));
            frogObject->getTransformation().addComponent(std::make_shared<Rotation>(270.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
        }
        frogObject->setTexture(frogTexture);
        drawableObjects.push_back(frogObject);
    }

    std::vector<float> treeData(tree, tree + sizeof(tree) / sizeof(float));
    treeModel = new Model();
    treeModel->loadFromData(treeData);

    std::vector<float> bushData(bushes, bushes + sizeof(bushes) / sizeof(float));
    bushModel = new Model();
    bushModel->loadFromData(bushData);

    float rectangleVertices[] = {
        -150.0f, 0.0f, -150.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        150.0f, 0.0f, -150.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        150.0f, 0.0f, 150.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,

        150.0f, 0.0f, 150.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
        -150.0f, 0.0f, 150.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
        -150.0f, 0.0f, -150.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    std::vector<float> rectangleData(
        rectangleVertices,
        rectangleVertices + sizeof(rectangleVertices) / sizeof(float));

    rectangleModel = new Model();
    rectangleModel->loadFromData(rectangleData, true);
    Texture *grassTexture = new Texture("textures/grass.png");

    DrawableObject *groundPlane = new DrawableObject(rectangleModel, phongShader);
    groundPlane->setColor(glm::vec3(1.0f));
    groundPlane->setTexture(grassTexture);
    drawableObjects.push_back(groundPlane);

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *treeObject = new DrawableObject(treeModel, phongShader);

        float posX = generator.getRandomFloatExcluding(-50.0f, 50.0f, -49.0f, 49.0f);
        float posZ = generator.getRandomFloatExcluding(-50.0f, 50.0f, -49.0f, 49.0f);
        float rotationAngle = generator.getRandomFloat(0.0f, 360.0f);
        float scaleValue = generator.getRandomFloat(0.5f, 2.0f);

        auto translation = std::make_shared<Translation>(glm::vec3(posX, 0.0f, posZ));
        auto rotation = std::make_shared<Rotation>(rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        auto scaling = std::make_shared<Scaling>(glm::vec3(scaleValue));

        treeObject->getTransformation().addComponent(scaling);
        treeObject->getTransformation().addComponent(rotation);
        treeObject->getTransformation().addComponent(translation);

        int randomChoice = generator.getRandomInt(0, 2);
        if (randomChoice == 0)
        {
            float angularVelocity = generator.getRandomFloat(8.0f, 20.0f);
            auto dynamicRotation = std::make_shared<DynamicRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), angularVelocity);
            treeObject->getTransformation().addComponent(dynamicRotation);
        }
        else if (randomChoice == 1)
        {
            // glm::vec3 velocity = generator.getRandomVec3(-1.0f, 1.0f, true);
            // velocity.y = 0.0f;
            // auto dynamicTranslation = std::make_shared<DynamicTranslation>(glm::vec3(0.0f), velocity);
            // treeObject->getTransformation().addComponent(dynamicTranslation);
        }

        treeObject->setColor(glm::vec3(0.1f, 1.0f, 0.1f));
        drawableObjects.push_back(treeObject);
    }

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *bushObject = new DrawableObject(bushModel, phongShader);

        float posX = generator.getRandomFloatExcluding(-50.0f, 50.0f, -49.0f, 49.0f);
        float posZ = generator.getRandomFloatExcluding(-50.0f, 50.0f, -49.0f, 49.0f);
        float rotationAngle = generator.getRandomFloat(0.0f, 360.0f);
        float scaleValue = generator.getRandomFloat(0.5f, 2.0f);

        auto translation = std::make_shared<Translation>(glm::vec3(posX, 0.0f, posZ));
        auto rotation = std::make_shared<Rotation>(rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        auto scaling = std::make_shared<Scaling>(glm::vec3(scaleValue));

        bushObject->getTransformation().addComponent(scaling);
        bushObject->getTransformation().addComponent(rotation);
        bushObject->getTransformation().addComponent(translation);

        bushObject->setColor(glm::vec3(0.3f, 1.0f, 1.1f));
        drawableObjects.push_back(bushObject);
    }

    lights.clear();
    lightSphereIndices.clear();

    std::vector<glm::vec3> lightColors = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f)};

    for (int i = 0; i < 6; ++i)
    {
        glm::vec3 randomPosition = generator.getRandomVec3(-4.0f, 4.0f, true);
        glm::vec3 randomVelocity = generator.getRandomVec3(1.0f, 10.0f, true);

        Light *light = new Light(POINT_LIGHT, randomPosition, lightColors[i], 1.0f);
        lights.push_back(light);

        DrawableObject *lightSphere = new DrawableObject(sphereModel, constantShader);

        auto dynamicTranslation = std::make_shared<DynamicTranslation>(randomPosition, randomVelocity);
        lightSphere->getTransformation().addComponent(dynamicTranslation);

        auto scaling = std::make_shared<Scaling>(glm::vec3(0.2f));
        lightSphere->getTransformation().addComponent(scaling);

        lightSphere->setColor(light->getColor());

        drawableObjects.push_back(lightSphere);
        lightSphereIndices.push_back(drawableObjects.size() - 1);
    }

    Light *spotLight = new Light(SPOTLIGHT, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);
    spotLight->setDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
    spotLight->setCutOffs(10.5f, 17.5f);
    lights.push_back(spotLight);

    Light *pointLight = new Light(POINT_LIGHT, glm::vec3(5.0f, 500.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 600.0f);
    lights.push_back(pointLight);

    flashLight = new FlashLight(camera, glm::vec3(1.0f), 1.0f, 7.5f, 14.5f);
    lights.push_back(flashLight);

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
}

void Scene::loadModelsScene2()
{
    drawableObjects.clear();
    lights.clear();
    lightSphereIndices.clear();

    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    DrawableObject *phongSphere = new DrawableObject(sphereModel, phongShader);
    phongSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));

    Material m2;
    m2.ambientStrength = 0.01f;
    m2.specularStrength = 0.8f;
    m2.shininess = 6.0f;
    phongSphere->setMaterial(m2);

    auto phongTranslation = std::make_shared<Translation>(glm::vec3(-2.0f, 0.0f, -2.0f));
    phongSphere->getTransformation().addComponent(phongTranslation);

    drawableObjects.push_back(phongSphere);

    DrawableObject *blinnSphere = new DrawableObject(sphereModel, phongShader);
    blinnSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));

    Material m1;
    m1.ambientStrength = 0.3f;
    m1.specularStrength = 0.2f;
    m1.shininess = 16.0f;
    blinnSphere->setMaterial(m1);

    auto blinnTranslation = std::make_shared<Translation>(glm::vec3(2.0f, 0.0f, -2.0f));
    blinnSphere->getTransformation().addComponent(blinnTranslation);

    drawableObjects.push_back(blinnSphere);

    DrawableObject *lambertSphere = new DrawableObject(sphereModel, phongShader);
    lambertSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));

    Material m3;
    m3.ambientStrength = 0.1f;
    m3.specularStrength = 0.5f;
    m3.shininess = 32.0f;
    lambertSphere->setMaterial(m3);

    auto lambertTranslation = std::make_shared<Translation>(glm::vec3(-2.0f, 0.0f, 2.0f));
    lambertSphere->getTransformation().addComponent(lambertTranslation);

    drawableObjects.push_back(lambertSphere);

    DrawableObject *constantSphere = new DrawableObject(sphereModel, phongShader);
    constantSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));

    Material m4;
    m4.ambientStrength = 1.1f;
    m4.specularStrength = 0.1f;
    m4.shininess = 100.0f;
    constantSphere->setMaterial(m4);

    auto constantTranslation = std::make_shared<Translation>(glm::vec3(2.0f, 0.0f, 2.0f));
    constantSphere->getTransformation().addComponent(constantTranslation);

    drawableObjects.push_back(constantSphere);

    Light *pointLight1 = new Light(POINT_LIGHT, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 10.0f);
    lights.push_back(pointLight1);

    Light *pointLight2 = new Light(POINT_LIGHT, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), 10.0f);
    lights.push_back(pointLight2);

    lightSphereIndices.push_back(drawableObjects.size() - 1);

    flashLight = new FlashLight(camera, glm::vec3(1.0f), 1.0f, 12.5f, 17.5f);
    lights.push_back(flashLight);

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
}

void Scene::loadModelsScene3()
{
    camera->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
    drawableObjects.clear();
    lights.clear();
    lightSphereIndices.clear();

    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    DrawableObject *sun = new DrawableObject(sphereModel, constantShader);
    sun->setColor(glm::vec3(1.0f, 1.0f, 0.0f));

    float sunScale = 3.0f;
    auto sunScaling = std::make_shared<Scaling>(glm::vec3(sunScale));
    sun->getTransformation().addComponent(sunScaling);

    drawableObjects.push_back(sun);

    Light *sunLight = new Light(POINT_LIGHT, glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.9f), 3.0f);
    lights.push_back(sunLight);

    const int numPlanets = 8;
    float planetSizes[numPlanets] = {0.383f, 0.949f, 1.0f, 0.832f, 1.809f, 1.449f, 1.407f, 1.383f};
    float orbitRadii[numPlanets] = {5.0f, 7.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f};

    for (int i = 0; i < 8; i++)
    {
        orbitRadii[i] *= 1.0f;
    }

    float orbitalSpeeds[numPlanets] = {40.87f, 40.02f, 50.78f, 38.13f, 42.07f, 45.69f, 36.81f, 45.43f};
    glm::vec3 planetColors[numPlanets] = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 0.8f, 0.6f),
        glm::vec3(0.2f, 0.6f, 1.0f),
        glm::vec3(1.0f, 0.5f, 0.5f),
        glm::vec3(1.0f, 0.9f, 0.6f),
        glm::vec3(0.9f, 0.8f, 0.5f),
        glm::vec3(0.5f, 0.8f, 1.0f),
        glm::vec3(0.3f, 0.5f, 0.8f)};

    for (int i = 0; i < numPlanets; ++i)
    {
        DrawableObject *planet = new DrawableObject(sphereModel, phongShader);
        planet->setColor(planetColors[i]);

        float planetScale = planetSizes[i];
        auto scaling = std::make_shared<Scaling>(glm::vec3(planetScale));

        float startAngle = glm::linearRand(0.0f, 360.0f);
        float angularVelocity = orbitalSpeeds[i] * 0.05f;
        auto dynamicRotation = std::make_shared<DynamicRotation>(startAngle, glm::vec3(0.0f, 1.0f, 0.0f), angularVelocity);
        planet->getTransformation().addComponent(dynamicRotation);

        auto translation = std::make_shared<Translation>(glm::vec3(orbitRadii[i], 0.0f, 0.0f));
        planet->getTransformation().addComponent(translation);
        planet->getTransformation().addComponent(scaling);

        if (i == 2)
        {
            float earthRotationSpeed = 50.0f;
            auto earthRotation = std::make_shared<DynamicRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), earthRotationSpeed);
            planet->getTransformation().addComponent(earthRotation);
        }

        drawableObjects.push_back(planet);

        if (i == 2)
        {
            DrawableObject *moon = new DrawableObject(sphereModel, phongShader);
            moon->setColor(glm::vec3(0.8f, 0.8f, 0.8f));

            float moonScale = 0.5f;
            auto moonScaling = std::make_shared<Scaling>(glm::vec3(moonScale));

            float moonAngularVelocity = 50.0f;
            auto moonDynamicRotation = std::make_shared<DynamicRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), moonAngularVelocity);

            float moonOrbitRadius = 5.0f;
            auto moonTranslation = std::make_shared<Translation>(glm::vec3(moonOrbitRadius, 0.0f, 0.0f));
            auto translation = std::make_shared<Translation>(glm::vec3(orbitRadii[i] * 2, 0.0f, 0.0f));

            moon->getTransformation().addComponent(moonScaling);
            moon->getTransformation().addComponent(dynamicRotation);
            moon->getTransformation().addComponent(translation);
            moon->getTransformation().addComponent(moonDynamicRotation);
            moon->getTransformation().addComponent(moonTranslation);

            drawableObjects.push_back(moon);
        }
    }

    flashLight = new FlashLight(camera, glm::vec3(1.0f), 70.0f, 4.5f, 8.5f);
    lights.push_back(flashLight);

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
}

void Scene::loadModelsScene4()
{
    drawableObjects.clear();
    lights.clear();
    lightSphereIndices.clear();

    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    int gridSize = 5;
    float spacing = 2.5f;

    for (int x = -gridSize; x <= gridSize; ++x)
    {
        for (int z = -gridSize; z <= gridSize; ++z)
        {
            DrawableObject *sphereObject = new DrawableObject(sphereModel, phongShader);
            sphereObject->setColor(glm::vec3(generator.getRandomFloat(0.5f, 1.0f),
                                             generator.getRandomFloat(0.5f, 1.0f),
                                             generator.getRandomFloat(0.5f, 1.0f)));

            glm::vec3 position = glm::vec3(x * spacing, 0.0f, z * spacing);
            auto translation = std::make_shared<Translation>(position);

            float scaleSpeed = generator.getRandomFloat(0.5f, 1.5f);
            auto dynamicScaling = std::make_shared<DynamicScaling>(glm::vec3(1.0f), scaleSpeed);
            sphereObject->getTransformation().addComponent(dynamicScaling);
            sphereObject->getTransformation().addComponent(translation);

            drawableObjects.push_back(sphereObject);
        }
    }

    flashLight = new FlashLight(camera, glm::vec3(1.0f), 70.0f, 4.5f, 8.5f);
    lights.push_back(flashLight);

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
}

void Scene::draw()
{
    if (skybox && skyboxFollowsCamera)
    {
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        skybox->draw(view, projection, skyboxFollowsCamera);
    }

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
    for (auto *object : drawableObjects)
    {
        object->draw();
    }

    if (skybox && !skyboxFollowsCamera)
    {
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        skybox->draw(view, projection, skyboxFollowsCamera);
    }
}

void Scene::update(float deltaTime)
{
    for (auto *object : drawableObjects)
    {
        object->update(deltaTime);
    }

    for (size_t i = 0; i < lightSphereIndices.size(); ++i)
    {
        size_t sphereIndex = lightSphereIndices[i];
        DrawableObject *lightSphere = drawableObjects[sphereIndex];

        auto dynamicTranslation = lightSphere->getTransformation().getComponent<DynamicTranslation>();

        if (dynamicTranslation)
        {
            glm::vec3 pos = dynamicTranslation->getPosition();
            glm::vec3 velocity = dynamicTranslation->getVelocity();

            float minX = -50.0f, maxX = 50.0f;
            float minY = 0.2f, maxY = 10.0f;
            float minZ = -50.0f, maxZ = 50.0f;

            bool velocityChanged = false;

            if (pos.x < minX)
            {
                pos.x = minX;
                velocity.x = -velocity.x;
                velocityChanged = true;
            }
            else if (pos.x > maxX)
            {
                pos.x = maxX;
                velocity.x = -velocity.x;
                velocityChanged = true;
            }

            if (pos.y < minY)
            {
                pos.y = minY;
                velocity.y = -velocity.y;
                velocityChanged = true;
            }
            else if (pos.y > maxY)
            {
                pos.y = maxY;
                velocity.y = -velocity.y;
                velocityChanged = true;
            }

            if (pos.z < minZ)
            {
                pos.z = minZ;
                velocity.z = -velocity.z;
                velocityChanged = true;
            }
            else if (pos.z > maxZ)
            {
                pos.z = maxZ;
                velocity.z = -velocity.z;
                velocityChanged = true;
            }

            if (velocityChanged)
            {
                dynamicTranslation->setVelocity(velocity);
            }

            lights[i]->setPositionOrDirection(pos);
        }
    }

    if (flashLight)
    {
        flashLight->setPositionOrDirection(camera->getPosition());
        flashLight->setDirection(camera->getFront());
    }
}

void Scene::toggleFlashLightIntensity()
{
    if (flashLight)
    {
        flashLight->toggleIntensity();
    }
}

void Scene::toggleSkyboxFollowing()
{
    skyboxFollowsCamera = !skyboxFollowsCamera;

    if (!skyboxFollowsCamera)
    {
        glm::vec3 cameraPos = camera->getPosition();
        glm::vec3 cameraFront = camera->getFront();
        glm::vec3 skyboxPosition = cameraPos + cameraFront * 50.0f;

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), skyboxPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(50.0f));

        skybox->setModelMatrix(modelMatrix);
    }

    if (skyboxFollowsCamera)
    {
        std::cout << "Skybox will now follow the camera." << std::endl;
    }
    else
    {
        std::cout << "Skybox is now stationary." << std::endl;
    }
}
