#include "Light.h"

Light::Light(LightType type, const glm::vec3& positionOrDirection, const glm::vec3& color, float intensity)
    : type(type), positionOrDirection(positionOrDirection), direction(glm::vec3(0.0f)),
      innerCutOff(0.0f), outerCutOff(0.0f),
      color(color), intensity(intensity), velocity(glm::vec3(0.0f))
{
}


Light::~Light()
{
}

void Light::setType(LightType type) {
    this->type = type;
}

LightType Light::getType() const {
    return type;
}

void Light::setPositionOrDirection(const glm::vec3& posOrDir) {
    this->positionOrDirection = posOrDir;
    notifyObservers();
}

glm::vec3 Light::getPositionOrDirection() const {
    return positionOrDirection;
}

void Light::setDirection(const glm::vec3& direction) {
    this->direction = direction;
    notifyObservers();
}

glm::vec3 Light::getDirection() const {
    return direction;
}

void Light::setCutOffs(float innerCutOffDegrees, float outerCutOffDegrees) {
    this->innerCutOff = glm::cos(glm::radians(innerCutOffDegrees));
    this->outerCutOff = glm::cos(glm::radians(outerCutOffDegrees));
    notifyObservers();
}

float Light::getInnerCutOff() const {
    return innerCutOff;
}

float Light::getOuterCutOff() const {
    return outerCutOff;
}

void Light::setColor(const glm::vec3& color)
{
    this->color = color;
    notifyObservers();
}

glm::vec3 Light::getColor() const
{
    return color;
}

void Light::setIntensity(float intensity)
{
    this->intensity = intensity;
    notifyObservers();
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
    positionOrDirection += velocity * deltaTime;
    notifyObservers();
}

void Light::notifyObservers()
{
    Subject::notifyObservers();
}
