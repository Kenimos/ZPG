#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include "Observer.h"
#include "Light.h"
#include "Camera.h"

class ShaderProgram : public Observer
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void use();

    void setMat4(const std::string &name, const glm::mat4 &mat);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setFloat(const std::string &name, float value);

    void update(Subject *subject) override;
    void setProgramID(GLuint programID);
    void retrieveUniformLocations();

    void setInt(const std::string &name, int value);

    void updateLightUniforms(Light *light);
    int getLightIndex(Light *light);
    void setLights(const std::vector<Light *> &lights);

private:
    GLuint programID;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
    std::unordered_map<std::string, GLint> uniformLocations;
    std::vector<Light *> lights;
};

#endif
