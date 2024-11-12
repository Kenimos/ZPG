

#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity)
    : position(position), color(color), intensity(intensity), velocity(0.0f)
{
}

void Light::setPosition(const glm::vec3& position)
{
    this->position = position;
    notifyObservers();
}

void Light::setColor(const glm::vec3& color)
{
    this->color = color;
    notifyObservers();
}

void Light::setIntensity(float intensity)
{
    this->intensity = intensity;
    notifyObservers();
}

glm::vec3 Light::getPosition() const
{
    return position;
}

glm::vec3 Light::getColor() const
{
    return color;
}

float Light::getIntensity() const
{
    return intensity;
}

void Light::setVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}

glm::vec3 Light::getVelocity() const
{
    return velocity;
}

void Light::update(float deltaTime)
{
    position += velocity * deltaTime;
    notifyObservers(); 
}

void Light::notifyObservers()
{
    Subject::notifyObservers();
}
