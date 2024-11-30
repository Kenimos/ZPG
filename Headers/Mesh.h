#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h> // Adjust if necessary

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    // Constructor
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    // Render the mesh
    void draw();

private:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Render data
    GLuint VAO, VBO, EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh();
};

#endif // MESH_H
