#include "FlashLight.h"

FlashLight::FlashLight(Camera *camera, const glm::vec3 &color, float intensity)
    : Light(camera->getPosition(), color, intensity), camera(camera)
{
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
        setPosition(camera->getPosition());
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
        setIntensity(1.0f);
    }
}