#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Subject.h"

enum LightType {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOTLIGHT
};

class Light : public Subject {
public:
    Light(LightType type, const glm::vec3& positionOrDirection, const glm::vec3& color, float intensity);
    ~Light();

    void setType(LightType type);
    LightType getType() const;

    void setPositionOrDirection(const glm::vec3& posOrDir);
    glm::vec3 getPositionOrDirection() const;

    void setDirection(const glm::vec3& direction);
    glm::vec3 getDirection() const;

    void setCutOffs(float innerCutOff, float outerCutOff);
    float getInnerCutOff() const;
    float getOuterCutOff() const;

    // Existing methods
    void setColor(const glm::vec3& color);
    glm::vec3 getColor() const;

    void setIntensity(float intensity);
    float getIntensity() const;

    void setVelocity(const glm::vec3& velocity);
    glm::vec3 getVelocity() const;

    void update(float deltaTime);
    void notifyObservers();

private:
    LightType type;
    glm::vec3 positionOrDirection;
    glm::vec3 direction;
    float innerCutOff;
    float outerCutOff;

    glm::vec3 color;
    float intensity;
    glm::vec3 velocity;
};

#endif // LIGHT_H
