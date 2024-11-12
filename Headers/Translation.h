// Translation.h
#pragma once

#include "TransformationComponent.h"
#include <glm/glm.hpp>

class Translation : public TransformationComponent {
public:
    Translation(const glm::vec3& translation);
    glm::mat4 getMatrix() const override;

private:
    glm::vec3 translation;
};
