// Rotation.h

#ifndef ROTATION_H
#define ROTATION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Rotation
{
public:
    Rotation();
    ~Rotation();

    void setRotation(float angleDegrees, const glm::vec3& axis);
    glm::quat getOrientation() const;

private:
    glm::quat orientation;
};

#endif // ROTATION_H
