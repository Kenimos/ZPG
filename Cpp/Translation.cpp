// Translation.cpp
#include "Translation.h"
#include <glm/gtc/matrix_transform.hpp>

Translation::Translation(const glm::vec3& translation)
    : translation(translation) {}

glm::mat4 Translation::getMatrix() const {
    return glm::translate(glm::mat4(1.0f), translation);
}

DynamicTranslation::DynamicTranslation(const glm::vec3& startPosition, const glm::vec3& velocity)
    : position(startPosition), velocity(velocity)
{
}

glm::mat4 DynamicTranslation::getMatrix() const
{
    return glm::translate(glm::mat4(1.0f), position);
}

void DynamicTranslation::update(float deltaTime)
{
    position += velocity * deltaTime;
}

glm::vec3  DynamicTranslation::getPosition() const
{
    return position;
}

glm::vec3 DynamicTranslation::getVelocity() const
{
    return velocity;
}

void DynamicTranslation::setVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}