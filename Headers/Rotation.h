// Rotation.h
#pragma once

#include "TransformationComponent.h"
#include <glm/glm.hpp>

class Rotation : public TransformationComponent {
public:
    Rotation(float angleDegrees, const glm::vec3& axis);
    glm::mat4 getMatrix() const override;

private:
    float angleDegrees; 
    glm::vec3 axis;
};

class DynamicRotation : public TransformationComponent
{
public:
    DynamicRotation(float startAngleDegrees, const glm::vec3& axis, float angularVelocity);
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;

private:
    float angleDegrees;
    glm::vec3 axis;
    float angularVelocity; 
};
