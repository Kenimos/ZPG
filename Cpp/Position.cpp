

#include "Position.h"

Position::Position()
    : position(0.0f, 0.0f, 0.0f)
{
}

Position::~Position()
{
}

void Position::setPosition(const glm::vec3 &position)
{
    this->position = position;
}

glm::vec3 Position::getPosition() const
{
    return position;
}
