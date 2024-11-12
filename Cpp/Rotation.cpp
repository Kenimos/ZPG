// Rotation.cpp
#include "Rotation.h"
#include <glm/gtc/matrix_transform.hpp>

Rotation::Rotation(float angleDegrees, const glm::vec3& axis)
    : angleDegrees(angleDegrees), axis(glm::normalize(axis)) {}

glm::mat4 Rotation::getMatrix() const {
    return glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
}
