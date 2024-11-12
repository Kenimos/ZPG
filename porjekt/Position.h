// Position.h

#ifndef POSITION_H
#define POSITION_H

#include <glm/glm.hpp>

class Position
{
public:
    Position();
    ~Position();

    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;

private:
    glm::vec3 position;
};

#endif // POSITION_H
