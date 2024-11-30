#include "Scaling.h"
#include <glm/gtc/matrix_transform.hpp>

Scaling::Scaling(const glm::vec3& scalingFactors)
    : scalingFactors(scalingFactors) {}

glm::mat4 Scaling::getMatrix() const {
    return glm::scale(glm::mat4(1.0f), scalingFactors);
}

DynamicScaling::DynamicScaling(const glm::vec3& startScale, float scaleSpeed)
    : scaleFactors(startScale), scaleSpeed(scaleSpeed)
{
}

glm::mat4 DynamicScaling::getMatrix() const
{
    return glm::scale(glm::mat4(1.0f), scaleFactors);
}

void DynamicScaling::update(float deltaTime)
{
    timeAccumulator += deltaTime;
    float scale = 1.0f + 0.5f * sin(timeAccumulator * scaleSpeed);
    scaleFactors = glm::vec3(scale);
}

void DynamicScaling::setScaleSpeed(float speed)
{
    scaleSpeed = speed;
}

float DynamicScaling::getScaleSpeed() const
{
    return scaleSpeed;
}