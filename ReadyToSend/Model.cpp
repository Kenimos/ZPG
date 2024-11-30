

#include "Model.h"
#include <iostream>
#include <vector>

Model::Model() : VAO(0), VBO(0), vertexCount(0)
{
}

Model::~Model()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
    }
}

void Model::loadFromData(const std::vector<float> &data, bool hasTexCoords)
{
    size_t stride = hasTexCoords ? 8 : 6;
    size_t numVertices = data.size() / stride;

    std::vector<Vertex> vertices(numVertices);

    for (size_t i = 0; i < numVertices; ++i)
    {
        size_t index = i * stride;
        vertices[i].position[0] = data[index];
        vertices[i].position[1] = data[index + 1];
        vertices[i].position[2] = data[index + 2];
        vertices[i].normal[0] = data[index + 3];
        vertices[i].normal[1] = data[index + 4];
        vertices[i].normal[2] = data[index + 5];
        if (hasTexCoords)
        {
            vertices[i].texCoords[0] = data[index + 6];
            vertices[i].texCoords[1] = data[index + 7];
        }
        else
        {
            vertices[i].texCoords[0] = 0.0f;
            vertices[i].texCoords[1] = 0.0f;
        }
    }

    setupMesh(vertices, hasTexCoords);
}

void Model::setupMesh(const std::vector<Vertex> &vertices, bool hasTexCoords)
{
    vertexCount = vertices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    if (VAO == 0 || VBO == 0)
    {
        std::cerr << "ERROR: Failed to generate VAO or VBO." << std::endl;
        return;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));

    if (hasTexCoords)
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texCoords));
    }

    glBindVertexArray(0);
}

void Model::draw()
{
    if (VAO == 0)
    {
        std::cerr << "ERROR: Attempting to draw with invalid VAO." << std::endl;
        return;
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
    glBindVertexArray(0);
}
