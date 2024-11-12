

#include "Scale.h"

Scale::Scale()
    : scale(1.0f, 1.0f, 1.0f)
{
}

Scale::~Scale()
{
}

void Scale::setScale(const glm::vec3 &scale)
{
    this->scale = scale;
}

glm::vec3 Scale::getScale() const
{
    return scale;
}
