#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "TransformationComponent.h"

class Transformation
{
public:
    Transformation();
    void addComponent(std::shared_ptr<TransformationComponent> component);
    glm::mat4 getModelMatrix() const;
    void update(float deltaTime);

    template <typename T>
    std::shared_ptr<T> getComponent() const
    {
        for (const auto& component : components)
        {
            auto castedComponent = std::dynamic_pointer_cast<T>(component);
            if (castedComponent)
            {
                return castedComponent;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<TransformationComponent>> components;
};
