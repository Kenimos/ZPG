// DrawableObject.h
#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include <glm/glm.hpp>

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shaderProgram);
    ~DrawableObject();

    Transformation& getTransformation();

    void setColor(const glm::vec3& color);
    const glm::vec3& getColor() const;

    void setShaderProgram(ShaderProgram* shaderProgram);

    // Optional: Methods for dynamic transformations
    void setRotationSpeed(float speed);
    void setVelocity(const glm::vec3& velocity);
    void enableRotation(bool enable);
    void enableMovement(bool enable);

    void update(float deltaTime);
    void draw();

private:
    Model* model;
    ShaderProgram* shaderProgram;
    Transformation transformation;
    glm::vec3 color;

    // Dynamic transformation variables
    float rotationSpeed;
    glm::vec3 velocity;
    bool rotate;
    bool move;
};
