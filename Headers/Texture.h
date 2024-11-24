#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
    Texture(const std::string &path);
    ~Texture();

    void bind(GLenum textureUnit = GL_TEXTURE0);
    GLuint getID() const;

private:
    GLuint textureID;
};