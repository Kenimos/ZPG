#include "ShaderProgram.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#define MAX_LIGHTS 10

ShaderProgram::ShaderProgram() : programID(0)
{
}

ShaderProgram::~ShaderProgram()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

void ShaderProgram::retrieveUniformLocations()
{
    std::vector<std::string> uniforms = {
        "modelMatrix",
        "viewMatrix",
        "projectionMatrix",
        "materialColor",
        "viewPos",
        "numLights",
        "ambientStrength",
        "specularStrength",
        "materialShininess",
        "skybox",
        "view",          
        "projection",
        "useTexture",
        "texture1",
        "model"
    };

    for (const auto &name : uniforms)
    {
        GLint location = glGetUniformLocation(programID, name.c_str());
        uniformLocations[name] = location;
    }

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        std::string index = std::to_string(i);
        std::string baseName = "lights[" + index + "].";

        uniformLocations[baseName + "type"] = glGetUniformLocation(programID, (baseName + "type").c_str());
        uniformLocations[baseName + "position"] = glGetUniformLocation(programID, (baseName + "position").c_str());
        uniformLocations[baseName + "direction"] = glGetUniformLocation(programID, (baseName + "direction").c_str());
        uniformLocations[baseName + "color"] = glGetUniformLocation(programID, (baseName + "color").c_str());
        uniformLocations[baseName + "intensity"] = glGetUniformLocation(programID, (baseName + "intensity").c_str());
        uniformLocations[baseName + "innerCutOff"] = glGetUniformLocation(programID, (baseName + "innerCutOff").c_str());
        uniformLocations[baseName + "outerCutOff"] = glGetUniformLocation(programID, (baseName + "outerCutOff").c_str());
    }
}

void ShaderProgram::use()
{
    if (programID == 0)
    {
        std::cerr << "ERROR: Shader program is invalid (programID == 0)." << std::endl;
        return;
    }
    glUseProgram(programID);

    setMat4("viewMatrix", viewMatrix);
    setMat4("projectionMatrix", projectionMatrix);

    setVec3("viewPos", cameraPosition);

}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat)
{
    auto it = uniformLocations.find(name);
    if (it == uniformLocations.end() || it->second == -1)
    {
        return;
    }
    glUniformMatrix4fv(it->second, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &vec)
{
    auto it = uniformLocations.find(name);
    if (it == uniformLocations.end() || it->second == -1)
    {
        return;
    }
    glUniform3fv(it->second, 1, glm::value_ptr(vec));
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    auto it = uniformLocations.find(name);
    if (it == uniformLocations.end() || it->second == -1)
    {
        return;
    }
    glUniform1f(it->second, value);
}

void ShaderProgram::setInt(const std::string &name, int value)
{
    auto it = uniformLocations.find(name);
    if (it != uniformLocations.end() && it->second != -1)
    {
        glUniform1i(it->second, value);
    }
}

void ShaderProgram::setAmbientStrength(float strength)
{
    setFloat("ambientStrength", strength);
}

void ShaderProgram::setMaterialShininess(float shininess)
{
    setFloat("materialShininess", shininess);
}

void ShaderProgram::update(Subject *subject)
{
    Camera *camera = dynamic_cast<Camera *>(subject);
    if (camera)
    {
        viewMatrix = camera->getViewMatrix();
        projectionMatrix = camera->getProjectionMatrix();
        cameraPosition = camera->getPosition();

        use();
        setMat4("viewMatrix", viewMatrix);
        setMat4("projectionMatrix", projectionMatrix);
        setVec3("viewPos", cameraPosition);
    }
    else if (Light *light = dynamic_cast<Light *>(subject))
    {
        use();
        updateLightUniforms(light);
    }
    else
    {
        std::cerr << "ShaderProgram::update() called with unknown subject type." << std::endl;
    }
}

void ShaderProgram::updateLightUniforms(Light *light)
{
    if (lights.empty())
    {
        return;
    }

    int lightIndex = getLightIndex(light);
    if (lightIndex == -1)
    {
        std::cerr << "Light not found in ShaderProgram's lights vector." << std::endl;
        return;
    }

    std::string baseName = "lights[" + std::to_string(lightIndex) + "].";

    setInt(baseName + "type", static_cast<int>(light->getType()));
    setVec3(baseName + "color", light->getColor());
    setFloat(baseName + "intensity", light->getIntensity());

    if (light->getType() == POINT_LIGHT || light->getType() == SPOTLIGHT)
    {
        setVec3(baseName + "position", light->getPositionOrDirection());
    }

    if (light->getType() == DIRECTIONAL_LIGHT || light->getType() == SPOTLIGHT)
    {
        setVec3(baseName + "direction", glm::normalize(light->getDirection()));
    }

    if (light->getType() == SPOTLIGHT)
    {
        setFloat(baseName + "innerCutOff", light->getInnerCutOff());
        setFloat(baseName + "outerCutOff", light->getOuterCutOff());
    }
}

int ShaderProgram::getLightIndex(Light *light)
{
    for (size_t i = 0; i < lights.size(); ++i)
    {
        if (lights[i] == light)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void ShaderProgram::setProgramID(GLuint programID)
{
    this->programID = programID;
    retrieveUniformLocations();
}

void ShaderProgram::setLights(const std::vector<Light *> &lights)
{
    this->lights = lights;
    use();

    setInt("numLights", static_cast<int>(lights.size()));

    for (size_t i = 0; i < lights.size(); ++i)
    {
        std::string baseName = "lights[" + std::to_string(i) + "].";

        setInt(baseName + "type", static_cast<int>(lights[i]->getType()));

        if (lights[i]->getType() == POINT_LIGHT || lights[i]->getType() == SPOTLIGHT)
        {
            setVec3(baseName + "position", lights[i]->getPositionOrDirection());
        }

        if (lights[i]->getType() == DIRECTIONAL_LIGHT || lights[i]->getType() == SPOTLIGHT)
        {
            setVec3(baseName + "direction", glm::normalize(lights[i]->getDirection()));
        }

        if (lights[i]->getType() == SPOTLIGHT)
        {
            setFloat(baseName + "innerCutOff", lights[i]->getInnerCutOff());
            setFloat(baseName + "outerCutOff", lights[i]->getOuterCutOff());
        }

        setVec3(baseName + "color", lights[i]->getColor());
        setFloat(baseName + "intensity", lights[i]->getIntensity());
    }
}

void ShaderProgram::setBool(const std::string &name, bool value)
{
    auto it = uniformLocations.find(name);
    if (it != uniformLocations.end() && it->second != -1)
    {
        glUniform1i(it->second, (int)value);
    }
}
