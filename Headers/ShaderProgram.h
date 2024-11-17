#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "Light.h"
#include "FlashLight.h"

class ShaderProgram : public Observer
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void retrieveUniformLocations();
    void use();
    void setMat4(const std::string &name, const glm::mat4 &mat);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setFloat(const std::string &name, float value);
    void setInt(const std::string &name, int value);
    void setAmbientStrength(float strength);
    void setMaterialShininess(float shininess);

    void update(Subject *subject) override;

    void setProgramID(GLuint programID);

    void setLights(const std::vector<Light *> &lights);

private:
    GLuint programID;
    std::unordered_map<std::string, GLint> uniformLocations;

    // Camera and light properties
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;

    std::vector<Light *> lights;

    void updateLightUniforms(Light *light);
    int getLightIndex(Light *light);
};

#endif // SHADERPROGRAM_H