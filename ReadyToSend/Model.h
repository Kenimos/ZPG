// Model.h

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <cstddef> 

struct Vertex
{
    float position[3];
    float normal[3];
};

class Model
{
public:
    Model();
    ~Model();

    void loadFromData(const std::vector<float>& data);
    void draw();

private:
    GLuint VAO, VBO;
    size_t vertexCount;

    void setupMesh(const std::vector<Vertex>& vertices);
};

#endif // MODEL_H
