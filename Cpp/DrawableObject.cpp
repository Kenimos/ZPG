#include "DrawableObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(glm::vec3(1.0f))
{
    // Initialize default material properties
    material.ambientStrength = 0.1f;
    material.specularStrength = 0.5f;
    material.shininess = 32.0f;
}

DrawableObject::~DrawableObject()
{
}

Transformation& DrawableObject::getTransformation()
{
    return transformation;
}

void DrawableObject::setColor(const glm::vec3& color)
{
    this->color = color;
}

const glm::vec3& DrawableObject::getColor() const
{
    return color;
}

void DrawableObject::setShaderProgram(ShaderProgram* shaderProgram)
{
    this->shaderProgram = shaderProgram;
}

void DrawableObject::update(float deltaTime)
{
    transformation.update(deltaTime);
}

void DrawableObject::draw()
{
    if (!shaderProgram)
    {
        std::cerr << "ERROR: ShaderProgram is not set for DrawableObject." << std::endl;
        return;
    }

    // Use the shader program
    shaderProgram->use();

    // Set uniforms
    glm::mat4 modelMatrix = transformation.getModelMatrix();
    shaderProgram->setMat4("modelMatrix", modelMatrix);
    shaderProgram->setVec3("materialColor", color);
    shaderProgram->setFloat("materialShininess", material.shininess);
    shaderProgram->setFloat("ambientStrength", material.ambientStrength);
    shaderProgram->setFloat("specularStrength", material.specularStrength);

    // Now draw the model
    model->draw();

    // Reset the shader program
    glUseProgram(0);
}

glm::vec3 DrawableObject::getPosition() const
{
    glm::mat4 modelMatrix = transformation.getModelMatrix();
    return glm::vec3(modelMatrix[3]);
}

// Implement the setMaterial and getMaterial methods
void DrawableObject::setMaterial(const Material& material)
{
    this->material = material;
}

const Material& DrawableObject::getMaterial() const
{
    return material;
}
