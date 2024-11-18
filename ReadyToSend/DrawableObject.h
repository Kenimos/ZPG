// DrawableObject.h
#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include <glm/glm.hpp>
#include "Material.h"

// Remove rotationSpeed, velocity, rotate, move variables and methods
class DrawableObject
{
public:
    DrawableObject(Model *model, ShaderProgram *shaderProgram);
    ~DrawableObject();

    Transformation &getTransformation();
    void setColor(const glm::vec3 &color);
    const glm::vec3 &getColor() const;
    void setShaderProgram(ShaderProgram *shaderProgram);
    void update(float deltaTime);
    void draw();
    glm::vec3 getPosition() const;

    // Add methods to set/get material
    void setMaterial(const Material &material);
    const Material &getMaterial() const;

private:
    Model *model;
    ShaderProgram *shaderProgram;
    glm::vec3 color;
    Transformation transformation;
    Material material; 
};
