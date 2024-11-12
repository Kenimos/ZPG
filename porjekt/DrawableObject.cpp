
#include "DrawableObject.h"
#include <glm/gtc/matrix_transform.hpp>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(glm::vec3(1.0f)),
      rotationSpeed(0.0f), velocity(0.0f), rotate(false), move(false)
{
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
        float currentAngle = transformation.getRotationAngle();
        currentAngle += angleIncrement;
        transformation.setRotation(currentAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (move)
    {
        glm::vec3 newPosition = transformation.getPosition() + velocity * deltaTime;
        transformation.setPosition(newPosition);
    }
}

void DrawableObject::draw()
{
    shaderProgram->use();
    shaderProgram->setMat4("modelMatrix", transformation.getModelMatrix());
    shaderProgram->setVec3("materialColor", color);
    model->draw();
}
