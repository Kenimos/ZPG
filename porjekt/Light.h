// Light.h

#ifndef LIGHT_H
#define LIGHT_H

#include "Subject.h"
#include <glm/glm.hpp>

class Light : public Subject
{
public:
    Light(const glm::vec3& position, const glm::vec3& color, float intensity);

    void setPosition(const glm::vec3& position);
    void setColor(const glm::vec3& color);
    void setIntensity(float intensity);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    float getIntensity() const;

    void setVelocity(const glm::vec3& velocity);
    glm::vec3 getVelocity() const;

    void update(float deltaTime);

    void notifyObservers();

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    glm::vec3 velocity;
};

#endif // LIGHT_H
