// Scaling.cpp
#include "Scaling.h"
#include <glm/gtc/matrix_transform.hpp>

Scaling::Scaling(const glm::vec3& scalingFactors)
    : scalingFactors(scalingFactors) {}

glm::mat4 Scaling::getMatrix() const {
    return glm::scale(glm::mat4(1.0f), scalingFactors);
}
