#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

class SkyBox
{
public:
    SkyBox(const std::vector<std::string> &faces);
    ~SkyBox();

    void draw(const glm::mat4 &view, const glm::mat4 &projection, bool followCamera = true);
    void setModelMatrix(const glm::mat4 &model);

private:
    GLuint VAO, VBO;
    GLuint cubemapTexture;
    ShaderProgram *shaderProgram;
    glm::mat4 modelMatrix; 

    void setupSkybox();
    GLuint loadCubemap(const std::vector<std::string> &faces);
};

#endif
