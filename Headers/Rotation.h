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
