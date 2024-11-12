

#include "Rotation.h"

Rotation::Rotation()
    : orientation(glm::quat())
{
}

Rotation::~Rotation()
{
}

void Rotation::setRotation(float angleDegrees, const glm::vec3 &axis)
{
    orientation = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
}

glm::quat Rotation::getOrientation() const
{
    return orientation;
}
