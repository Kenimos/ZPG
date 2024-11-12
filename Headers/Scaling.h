// Scaling.h
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
