#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

// Forward declaration to avoid including Assimp headers in the header file
struct aiNode;
struct aiScene;
struct aiMesh;

class Model {
public:
    Model();
    ~Model();

    // Existing function to load model from data arrays
    void loadFromData(const std::vector<float>& data, bool hasTexCoords = false);

    // New function to load model from file using Assimp
    void loadModel(const std::string& path);

    // Function to draw the model
    void draw();

private:
    // Mesh data
    std::vector<Mesh> meshes;
    std::string directory;

    // Functions to process Assimp data (used only when loading from files)
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    // Function to set up the mesh when loading from data arrays
    void setupMesh(const std::vector<Vertex>& vertices, bool hasTexCoords);

    // Vertex count for drawing when loading from data arrays
    size_t vertexCount;

    // OpenGL buffers for when loading from data arrays
    GLuint VAO, VBO;

    // Flag to indicate which loading method was used
    bool loadedFromDataArray;
};

#endif // MODEL_H
