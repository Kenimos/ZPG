// Transformation.h
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "TransformationComponent.h"

class Transformation {
public:
    Transformation();
    ~Transformation();

    glm::mat4 getModelMatrix();

    void addTransformation(std::unique_ptr<TransformationComponent> component);
    void clearTransformations();

    void translate(const glm::vec3& translation);
    void rotate(float angleDegrees, const glm::vec3& axis);
    void scale(const glm::vec3& scalingFactors);

private:
    std::vector<std::unique_ptr<TransformationComponent>> components;
};
