#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include <glm/glm.hpp>
#include "Material.h"
#include "Texture.h"

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

    void setMaterial(const Material &material);
    const Material &getMaterial() const;
    void setTexture(Texture* texture);

private:
    Model *model;
    ShaderProgram *shaderProgram;
    glm::vec3 color;
    Transformation transformation;
    Material material;
    Texture* texture;
};
