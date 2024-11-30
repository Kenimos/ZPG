#pragma once

#include "TransformationComponent.h"
#include <glm/glm.hpp>

class Scaling : public TransformationComponent {
public:
    Scaling(const glm::vec3& scalingFactors);
    glm::mat4 getMatrix() const override;

private:
    glm::vec3 scalingFactors;
};

class DynamicScaling : public TransformationComponent
{
public:
    DynamicScaling(const glm::vec3& startScale, float scaleSpeed);
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;

    void setScaleSpeed(float speed);
    float getScaleSpeed() const;

private:
    glm::vec3 scaleFactors;
    float scaleSpeed;
    float timeAccumulator = 0.0f;
};
