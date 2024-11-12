

#include "Shader.h"
#include <iostream>

Shader::Shader(GLenum shaderType) : shaderID(0)
{
    shaderID = glCreateShader(shaderType);
}

Shader::~Shader()
{
    if (shaderID != 0)
    {
        glDeleteShader(shaderID);
    }
}

bool Shader::compile(const std::string &source)
{
    const char *sourceCStr = source.c_str();
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {

        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::string errorLog(maxLength, ' ');
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << errorLog << std::endl;
        return false;
    }

    return true;
}

GLuint Shader::getID() const
{
    return shaderID;
}
