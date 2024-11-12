// DrawableObject.cpp
#include "DrawableObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(glm::vec3(1.0f)),
      rotationSpeed(0.0f), velocity(0.0f), rotate(false), move(false)
{
}

DrawableObject::~DrawableObject()
{
    // Assuming Transformation manages its own memory via unique_ptr
}

Transformation& DrawableObject::getTransformation()
{
    return transformation;
}

void DrawableObject::setColor(const glm::vec3& color)
{
    this->color = color;
}

const glm::vec3& DrawableObject::getColor() const
{
    return color;
}

void DrawableObject::setShaderProgram(ShaderProgram* shaderProgram)
{
    this->shaderProgram = shaderProgram;
}

void DrawableObject::setRotationSpeed(float speed)
{
    this->rotationSpeed = speed;
}

void DrawableObject::setVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}

void DrawableObject::enableRotation(bool enable)
{
    this->rotate = enable;
}

void DrawableObject::enableMovement(bool enable)
{
    this->move = enable;
}

void DrawableObject::update(float deltaTime)
{
    if (rotate)
    {
        float angleIncrement = rotationSpeed * deltaTime; 
        transformation.rotate(angleIncrement, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (move)
    {
        glm::vec3 displacement = velocity * deltaTime;
        transformation.translate(displacement);
    }
}

void DrawableObject::draw()
{
    if (!shaderProgram)
    {
        std::cerr << "ERROR: ShaderProgram is not set for DrawableObject." << std::endl;
        return;
    }

    shaderProgram->use();
    glm::mat4 modelMatrix = transformation.getModelMatrix();
    shaderProgram->setMat4("modelMatrix", modelMatrix);
    shaderProgram->setVec3("materialColor", color);
    model->draw();
}
