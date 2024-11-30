#pragma once

#include "TransformationComponent.h"
#include <glm/glm.hpp>

class Translation : public TransformationComponent
{
public:
    Translation(const glm::vec3 &translation);
    glm::mat4 getMatrix() const override;

private:
    glm::vec3 translation;
};

class DynamicTranslation : public TransformationComponent
{
public:
    DynamicTranslation(const glm::vec3& startPosition, const glm::vec3& velocity);
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;
    glm::vec3  getPosition() const;
    glm::vec3  getVelocity() const;
    void setVelocity(const glm::vec3& velocity);

private:
    glm::vec3 position;
    glm::vec3 velocity;
};