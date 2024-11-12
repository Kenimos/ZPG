// Transformation.h
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transformation
{
public:
    Transformation();

    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const;

    void setRotation(float angleDegrees, const glm::vec3& axis);
    float getRotationAngle() const; 
    const glm::quat& getOrientation() const;

    void setScale(const glm::vec3& scale);
    const glm::vec3& getScale() const;

    glm::mat4 getModelMatrix() const;

private:
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

    float rotationAngleDegrees; 
    glm::vec3 rotationAxis;     
};
