#include "FlashLight.h"
#include <glm/gtc/matrix_transform.hpp>

FlashLight::FlashLight(Camera *camera, const glm::vec3 &color, float intensity, float innerCutOffDegrees, float outerCutOffDegrees)
    : Light(SPOTLIGHT, camera->getPosition(), color, intensity), camera(camera)
{
    setDirection(camera->getFront());
    setCutOffs(innerCutOffDegrees, outerCutOffDegrees);
    camera->attach(this);
}

FlashLight::~FlashLight()
{
    camera->detach(this);
}

void FlashLight::update(Subject *subject)
{
    if (subject == camera)
    {
        setPositionOrDirection(camera->getPosition());
        setDirection(camera->getFront());
        notifyObservers();
    }
}

void FlashLight::toggleIntensity()
{
    if (getIntensity() > 0.0f)
    {
        setIntensity(0.0f);
    }
    else
    {
        setIntensity(100.0f);
    }
}

void FlashLight::setCutOffs(float innerCutOffDegrees, float outerCutOffDegrees)
{
    Light::setCutOffs(innerCutOffDegrees, outerCutOffDegrees);
}
