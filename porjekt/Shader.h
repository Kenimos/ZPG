// Shader.h

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader
{
public:
    Shader(GLenum shaderType);
    ~Shader();

    bool compile(const std::string& source);
    GLuint getID() const;

private:
    GLuint shaderID;
};

#endif // SHADER_H
