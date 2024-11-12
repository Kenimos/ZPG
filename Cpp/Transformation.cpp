// Transformation.cpp
#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>

Transformation::Transformation()
    : position(0.0f), orientation(), scale(1.0f),
      rotationAngleDegrees(0.0f), rotationAxis(0.0f, 1.0f, 0.0f)
{
}

void Transformation::setPosition(const glm::vec3& position)
{
    this->position = position;
}

const glm::vec3& Transformation::getPosition() const
{
    return position;
}

void Transformation::setRotation(float angleDegrees, const glm::vec3& axis)
{
    rotationAngleDegrees = angleDegrees;
    rotationAxis = glm::normalize(axis);
    orientation = glm::angleAxis(glm::radians(angleDegrees), rotationAxis);
}

float Transformation::getRotationAngle() const
{
    return rotationAngleDegrees;
}

const glm::quat& Transformation::getOrientation() const
{
    return orientation;
}

void Transformation::setScale(const glm::vec3& scale)
{
    this->scale = scale;
}

const glm::vec3& Transformation::getScale() const
{
    return scale;
}

glm::mat4 Transformation::getModelMatrix() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model *= glm::mat4_cast(orientation);
    model = glm::scale(model, scale);
    return model;
}
