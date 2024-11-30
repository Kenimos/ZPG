#pragma once

#include <glm/glm.hpp>

class TransformationComponent
{
public:
    virtual ~TransformationComponent() = default;
    virtual glm::mat4 getMatrix() const = 0;
    virtual void update(float deltaTime) {}
};