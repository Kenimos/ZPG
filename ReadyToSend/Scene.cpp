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

Scene::Scene(Camera *camera)
    : camera(camera),
      treeModel(nullptr), bushModel(nullptr), sphereModel(nullptr), giftModel(nullptr), rectangleModel(nullptr),
      constantShader(nullptr), lambertShader(nullptr), phongShader(nullptr), blinnPhongShader(nullptr),
      spotlightShader(nullptr), currentShaderIndex(0), flashLight(nullptr)
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

    
    std::vector<float> treeData(tree, tree + sizeof(tree) / sizeof(float));
    treeModel = new Model();
    treeModel->loadFromData(treeData);

    std::vector<float> bushData(bushes, bushes + sizeof(bushes) / sizeof(float));
    bushModel = new Model();
    bushModel->loadFromData(bushData);

    
    float rectangleVertices[] = {
        
        -50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f,
        50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f,
        50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f,

        50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f,
        -50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f,
        -50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f};

    std::vector<float> rectangleData(
        rectangleVertices,
        rectangleVertices + sizeof(rectangleVertices) / sizeof(float));

    rectangleModel = new Model();
    rectangleModel->loadFromData(rectangleData);

    
    DrawableObject *groundPlane = new DrawableObject(rectangleModel, phongShader);
    groundPlane->setColor(glm::vec3(0.5f, 0.5f, 0.5f)); 
    drawableObjects.push_back(groundPlane);

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *treeObject = new DrawableObject(treeModel, phongShader);

        float posX = generator.getRandomFloat(-50.0f, 50.0f);
        float posZ = generator.getRandomFloat(-50.0f, 50.0f);
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
            glm::vec3 velocity = generator.getRandomVec3(-1.0f, 1.0f, true);
            velocity.y = 0.0f;
            auto dynamicTranslation = std::make_shared<DynamicTranslation>(glm::vec3(0.0f), velocity);
            treeObject->getTransformation().addComponent(dynamicTranslation);
        }

        treeObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        drawableObjects.push_back(treeObject);
    }

    for (int i = 0; i < 50; ++i)
    {
        DrawableObject *bushObject = new DrawableObject(bushModel, phongShader);

        float posX = generator.getRandomFloat(-50.0f, 50.0f);
        float posZ = generator.getRandomFloat(-50.0f, 50.0f);
        float rotationAngle = generator.getRandomFloat(0.0f, 360.0f);
        float scaleValue = generator.getRandomFloat(0.5f, 2.0f);

        
        auto translation = std::make_shared<Translation>(glm::vec3(posX, 0.0f, posZ));
        auto rotation = std::make_shared<Rotation>(rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        auto scaling = std::make_shared<Scaling>(glm::vec3(scaleValue));

        bushObject->getTransformation().addComponent(scaling);
        bushObject->getTransformation().addComponent(rotation);
        bushObject->getTransformation().addComponent(translation);

        bushObject->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
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

        Light *light = new Light(POINT_LIGHT, randomPosition, lightColors[i], 15.0f);
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
    
    Light *spotLight = new Light(SPOTLIGHT, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 500.0f);
    spotLight->setDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
    spotLight->setCutOffs(10.5f, 17.5f);
    lights.push_back(spotLight);

    Light *pointLight = new Light(POINT_LIGHT, glm::vec3(5.0f, 50.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 300.0f);
    lights.push_back(pointLight);

    flashLight = new FlashLight(camera, glm::vec3(1.0f), 100.0f, 7.5f, 14.5f);
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
    drawableObjects.clear();
    lights.clear();
    lightSphereIndices.clear();
    
    if (!sphereModel)
    {
        std::vector<float> sphereData(sphere, sphere + sizeof(sphere) / sizeof(float));
        sphereModel = new Model();
        sphereModel->loadFromData(sphereData);
    }

    
    DrawableObject *centralSphere = new DrawableObject(sphereModel, phongShader);
    centralSphere->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

    auto centralScaling = std::make_shared<Scaling>(glm::vec3(2.0f));
    centralSphere->getTransformation().addComponent(centralScaling);

    drawableObjects.push_back(centralSphere);

    int numSpheres = 8;
    float radius = 10.0f;

    for (int i = 0; i < numSpheres; ++i)
    {
        DrawableObject *orbitingSphere = new DrawableObject(sphereModel, phongShader);
        orbitingSphere->setColor(glm::vec3(generator.getRandomFloat(0.5f, 1.0f),
                                           generator.getRandomFloat(0.5f, 1.0f),
                                           generator.getRandomFloat(0.5f, 1.0f)));

        float angle = (360.0f / numSpheres) * i;
        auto rotation = std::make_shared<Rotation>(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        auto dynamicRotation = std::make_shared<DynamicRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 20.0f);

        auto translation = std::make_shared<Translation>(glm::vec3(radius, 0.0f, 0.0f));

        orbitingSphere->getTransformation().addComponent(dynamicRotation);
        orbitingSphere->getTransformation().addComponent(rotation);
        orbitingSphere->getTransformation().addComponent(translation);

        drawableObjects.push_back(orbitingSphere);
    }

    
    Light *centralLight = new Light(POINT_LIGHT, glm::vec3(0.0f), glm::vec3(1.0f), 50.0f);
    lights.push_back(centralLight);

    
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
