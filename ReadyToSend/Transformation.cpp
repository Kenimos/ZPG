#include "Transformation.h"

Transformation::Transformation()
{
}

void Transformation::addComponent(std::shared_ptr<TransformationComponent> component)
{
    components.push_back(component);
}

glm::mat4 Transformation::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    for (const auto& component : components)
    {
        model = model * component->getMatrix();
    }
    return model;
}

void Transformation::update(float deltaTime)
{
    for (const auto& component : components)
    {
        component->update(deltaTime);
    }
}
