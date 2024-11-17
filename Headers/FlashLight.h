
#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "Light.h"
#include "Observer.h"
#include "Subject.h"
#include "Camera.h"

class FlashLight : public Light, public Observer
{
public:
    FlashLight(Camera *camera, const glm::vec3 &color, float intensity, float innerCutoff, float outerCutoff);
    ~FlashLight();

    void update(Subject *subject) override;

    void toggleIntensity();

    void setCutOffs(float innerCutOffDegrees, float outerCutOffDegrees);

    glm::vec3 getDirection() const;

private:
    Camera *camera;
};

#endif
