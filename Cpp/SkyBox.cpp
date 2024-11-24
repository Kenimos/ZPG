#define STB_IMAGE_IMPLEMENTATION
#include "SkyBox.h"
#include "ShaderLoader.h"
#include "../stb/stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const float skyboxVertices[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

SkyBox::SkyBox(const std::vector<std::string> &faces)
{
    setupSkybox();
    cubemapTexture = loadCubemap(faces);

    ShaderLoader shaderLoader;
    GLuint skyboxShaderID = shaderLoader.loadShader("Shaders/skybox_vertex.glsl", "Shaders/skybox_fragment.glsl");
    if (skyboxShaderID == 0)
    {
        std::cerr << "Failed to load skybox shaders." << std::endl;
    }
    shaderProgram = new ShaderProgram();
    shaderProgram->setProgramID(skyboxShaderID);
    modelMatrix = glm::mat4(1.0f);
}

SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &cubemapTexture);
    delete shaderProgram;
}

void SkyBox::setupSkybox()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

GLuint SkyBox::loadCubemap(const std::vector<std::string> &faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            else
                format = GL_RGB;

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            std::cout << "Loaded cubemap texture: " << faces[i] << std::endl;
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    return textureID;
}

void SkyBox::draw(const glm::mat4 &view, const glm::mat4 &projection, bool followCamera)
{
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    GLint depthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

    if (followCamera)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    glDepthFunc(GL_LEQUAL);
    if (followCamera)
    {
        glDepthMask(GL_FALSE);
    }

    shaderProgram->use();

    glm::mat4 viewMatrix = followCamera ? glm::mat4(glm::mat3(view)) : view;
    glm::mat4 modelMatrix = followCamera ? glm::mat4(1.0f) : this->modelMatrix;

    shaderProgram->setMat4("view", viewMatrix);
    shaderProgram->setMat4("projection", projection);
    shaderProgram->setMat4("model", modelMatrix);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    shaderProgram->setInt("skybox", 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    if (followCamera)
    {
        glDepthMask(depthMaskEnabled);
    }
    glDepthFunc(depthFunc);

    if (cullFaceEnabled)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(cullFaceMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}


void SkyBox::setModelMatrix(const glm::mat4 &model)
{
    modelMatrix = model;
}
