// Transformation.cpp

#include "Transformation.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"

Transformation::Transformation() {
}

Transformation::~Transformation() {
    clearTransformations();
}

glm::mat4 Transformation::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    // Apply transformations in reverse order (last added transformation is applied first)
    for (auto it = components.rbegin(); it != components.rend(); ++it) {
        model = (*it)->getMatrix() * model;
    }
    return model;
}

void Transformation::addTransformation(std::unique_ptr<TransformationComponent> component) {
    components.push_back(std::move(component));
}

void Transformation::clearTransformations() {
    components.clear(); // Automatically deletes components due to unique_ptr
}

void Transformation::translate(const glm::vec3& translation) {
    addTransformation(std::make_unique<Translation>(translation));
}

void Transformation::rotate(float angleDegrees, const glm::vec3& axis) {
    addTransformation(std::make_unique<Rotation>(angleDegrees, axis));
}

void Transformation::scale(const glm::vec3& scalingFactors) {
    addTransformation(std::make_unique<Scaling>(scalingFactors));
}
