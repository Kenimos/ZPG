#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <GL/glew.h>

Model::Model() : vertexCount(0), VAO(0), VBO(0), loadedFromDataArray(false)
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
    loadedFromDataArray = true;
    size_t stride = hasTexCoords ? 8 : 6;
    size_t numVertices = data.size() / stride;

    std::vector<Vertex> vertices(numVertices);

    for (size_t i = 0; i < numVertices; ++i)
    {
        size_t index = i * stride;
        vertices[i].position = glm::vec3(data[index], data[index + 1], data[index + 2]);
        vertices[i].normal = glm::vec3(data[index + 3], data[index + 4], data[index + 5]);
        if (hasTexCoords)
        {
            vertices[i].texCoords = glm::vec2(data[index + 6], data[index + 7]);
        }
        else
        {
            vertices[i].texCoords = glm::vec2(0.0f);
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

void Model::loadModel(const std::string &path)
{
    loadedFromDataArray = false;

    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_CalcTangentSpace |
                                                 aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        else
        {
            vertex.normal = glm::vec3(0.0f);
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    meshes.push_back(Mesh(vertices, indices));
}

void Model::draw()
{
    if (loadedFromDataArray)
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
    else
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
    }
}
