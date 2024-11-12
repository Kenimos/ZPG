

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"

class DrawableObject
{
public:
    DrawableObject(Model *model, ShaderProgram *shaderProgram);

    Transformation &getTransformation();

    void setColor(const glm::vec3 &color);
    void setShaderProgram(ShaderProgram *shaderProgram);
    const glm::vec3 &getColor() const;

    void setRotationSpeed(float speed);
    void setVelocity(const glm::vec3 &velocity);
    void enableRotation(bool enable);
    void enableMovement(bool enable);

    void update(float deltaTime);

    void draw();

private:
    Model *model;
    Transformation transformation;
    ShaderProgram *shaderProgram;
    glm::vec3 color;

    float rotationSpeed;
    glm::vec3 velocity;
    bool rotate;
    bool move;
};

#endif
