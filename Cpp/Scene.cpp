

#include "Scene.h"
#include "../Models/tree.h"
#include "../Models/bushes.h"
#include "../Models/sphere.h"
#include "../Models/gift.h"
#include <random>
#include <glm/glm.hpp>
#include <iostream>
#include "ShaderLoader.h"

Scene::Scene(Camera *camera)
    : camera(camera),
      treeModel(nullptr), bushModel(nullptr), sphereModel(nullptr), giftModel(nullptr),
      constantShader(nullptr), lambertShader(nullptr), phongShader(nullptr), blinnPhongShader(nullptr),
      currentShaderIndex(0)
{
}

Scene::~Scene()
{
    delete treeModel;
    delete bushModel;
    delete sphereModel;
    delete giftModel;

    delete constantShader;
    delete lambertShader;
    delete phongShader;
    delete blinnPhongShader;

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

    for (auto *shader : shaders)
    {
        camera->attach(shader);
    }

    camera->notifyObservers();
}

void Scene::switchShaders()
{
    currentShaderIndex = (currentShaderIndex + 1) % shaders.size();

    ShaderProgram *newShader = shaders[currentShaderIndex];

    for (auto *object : drawableObjects)
    {
        object->setShaderProgram(newShader);
    }
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

    std::vector<float> treeData(tree, tree + sizeof(tree) / sizeof(float));
    treeModel = new Model();
    treeModel->loadFromData(treeData);

    std::vector<float> bushData(bushes, bushes + sizeof(bushes) / sizeof(float));
    bushModel = new Model();
    bushModel->loadFromData(bushData);

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *treeObject = new DrawableObject(treeModel, phongShader);

        float posX = generator.getRandomFloat(-50.0f, 50.0f);
        float posZ = generator.getRandomFloat(-50.0f, 50.0f);
        treeObject->getTransformation().setPosition(glm::vec3(posX, 0.0f, posZ));

        float rotationAngle = generator.getRandomFloat(-50.0f, 50.0f);
        treeObject->getTransformation().setRotation(rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        float scaleValue = generator.getRandomFloat(0.5f, 2.0f);
        treeObject->getTransformation().setScale(glm::vec3(scaleValue));
        treeObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

        int randomChoice = generator.getRandomInt(0, 2);
        if (randomChoice == 0)
        {
            treeObject->enableRotation(true);
            float rotationSpeed = generator.getRandomFloat(8.0f, 20.0f);
            treeObject->setRotationSpeed(rotationSpeed);
        }
        else if (randomChoice == 1 )
        {
            treeObject->enableMovement(true);
            glm::vec3 velocity(generator.getRandomVec3(-1.0f, 1.0f, true));
            treeObject->setVelocity(velocity);
        }

        drawableObjects.push_back(treeObject);
    }

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *bushObject = new DrawableObject(bushModel, phongShader);

        float posX = generator.getRandomFloat(-50.0f, 50.0f);
        float posZ = generator.getRandomFloat(-50.0f, 50.0f);
        bushObject->getTransformation().setPosition(glm::vec3(posX, 0.0f, posZ));

        float rotationAngle = generator.getRandomFloat(-50.0f, 50.0f);
        bushObject->getTransformation().setRotation(rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        float scaleValue = generator.getRandomFloat(0.5f, 2.0f);
        bushObject->getTransformation().setScale(glm::vec3(scaleValue));
        bushObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

        drawableObjects.push_back(bushObject);
    }

    lights.clear();

    Light *light1 = new Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 5.0f);
    Light *light2 = new Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    Light *light3 = new Light(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);

    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);
}

void Scene::loadModelsScene2()
{
    std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
    sphereModel = new Model();
    sphereModel->loadFromData(sphereData);

    DrawableObject *phongSphere = new DrawableObject(sphereModel, phongShader);
    phongSphere->getTransformation().setPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
    phongSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));
    drawableObjects.push_back(phongSphere);

    DrawableObject *blinnSphere = new DrawableObject(sphereModel, blinnPhongShader);
    blinnSphere->getTransformation().setPosition(glm::vec3(2.0f, 0.0f, -2.0f));
    blinnSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));
    drawableObjects.push_back(blinnSphere);

    DrawableObject *lambertSphere = new DrawableObject(sphereModel, lambertShader);
    lambertSphere->getTransformation().setPosition(glm::vec3(-2.0f, 0.0f, 2.0f));
    lambertSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));
    drawableObjects.push_back(lambertSphere);

    DrawableObject *constantSphere = new DrawableObject(sphereModel, constantShader);
    constantSphere->getTransformation().setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    constantSphere->setColor(glm::vec3(0.9f, 0.9f, 1.0f));
    drawableObjects.push_back(constantSphere);

    lights.clear();

    Light *light1 = new Light(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    Light *light2 = new Light(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    Light *light3 = new Light(glm::vec3(0.0f, 14.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);

    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);
}

void Scene::loadModelsScene3()
{

    std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
    sphereModel = new Model();
    sphereModel->loadFromData(sphereData);

    DrawableObject *sphereObject = new DrawableObject(sphereModel, phongShader);
    sphereObject->getTransformation().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sphereObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    drawableObjects.push_back(sphereObject);

    lights.clear();
    lightSphereIndices.clear();

    

    std::vector<glm::vec3> lightColors = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)};

    for (int i = 0; i < 3; ++i)
    {
        glm::vec3 randomPosition(generator.getRandomVec3(-4.0f, 4.0f, true));
        glm::vec3 randomVelocity(generator.getRandomVec3(1.0f, 10.0f, true));

        Light *light = new Light(randomPosition, lightColors[i], 4.0f);
        light->setVelocity(randomVelocity);

        lights.push_back(light);

        DrawableObject *lightSphere = new DrawableObject(sphereModel, constantShader);
        lightSphere->getTransformation().setPosition(light->getPosition());
        lightSphere->getTransformation().setScale(glm::vec3(0.2f));
        lightSphere->setColor(light->getColor());
        drawableObjects.push_back(lightSphere);

        lightSphereIndices.push_back(drawableObjects.size() - 1);
    }
}

void Scene::loadModelsScene4()
{

    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    DrawableObject *sphereObject = new DrawableObject(sphereModel, phongShader);
    sphereObject->getTransformation().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sphereObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    drawableObjects.push_back(sphereObject);

    lights.clear();

    flashLight = new FlashLight(camera, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    lights.push_back(flashLight);

    flashLight->attach(spotlightShader);

    flashLight->notifyObservers();

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }
}

void Scene::draw()
{

    for (auto *shader : shaders)
    {
        shader->setLights(lights);
    }

    for (auto *object : drawableObjects)
    {
        object->draw();
    }
}

void Scene::update(float deltaTime)
{

    for (auto *object : drawableObjects)
    {
        object->update(deltaTime);
    }

    for (size_t i = 0; i < lights.size(); ++i)
    {
        lights[i]->update(deltaTime);

        glm::vec3 pos = lights[i]->getPosition();
        glm::vec3 velocity = lights[i]->getVelocity();

        float minX = -5.0f, maxX = 5.0f;
        float minY = -5.0f, maxY = 5.0f;
        float minZ = -5.0f, maxZ = 5.0f;

        if (pos.x < minX || pos.x > maxX)
        {
            velocity.x = -velocity.x;
            lights[i]->setVelocity(velocity);
        }
        if (pos.y < minY || pos.y > maxY)
        {
            velocity.y = -velocity.y;
            lights[i]->setVelocity(velocity);
        }
        if (pos.z < minZ || pos.z > maxZ)
        {
            velocity.z = -velocity.z;
            lights[i]->setVelocity(velocity);
        }

        lights[i]->setPosition(pos);

        if (i < lightSphereIndices.size())
        {
            drawableObjects[lightSphereIndices[i]]->getTransformation().setPosition(lights[i]->getPosition());
        }
    }
}

void Scene::toggleFlashLightIntensity()
{
    if (lights.empty())
    {
        return;
    }

    FlashLight* flashLightPtr = dynamic_cast<FlashLight*>(lights[0]);
    if (flashLightPtr)
    {
        flashLightPtr->toggleIntensity();
    }
    else
    {
        std::cerr << "First light is not a FlashLight." << std::endl;
    }
}