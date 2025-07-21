#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assetutils.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};


class Mesh {
    friend class MeshRenderer;
public:
    Mesh(const std::string& filename);
    ~Mesh();

    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;


    // Default PBR values
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    // Rendering
    void Draw() const;
    
private:
    // OpenGL objects
    unsigned int VAO, VBO, EBO;
    unsigned int textureIndex[(unsigned long)TextureType::MAX_TEXTURE_TYPES] = {0};
    
    // Setup functions
    void SetupMesh();
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
}; 