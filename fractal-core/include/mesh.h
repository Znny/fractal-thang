#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assetutils.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(const std::string& filename);
    ~Mesh();

    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    // PBR texture IDs
    unsigned int albedoTexture;
    unsigned int normalTexture;
    unsigned int metallicTexture;
    unsigned int roughnessTexture;
    unsigned int aoTexture;
    
    // Default PBR values
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    // Rendering
    void Draw() const;
    
    // Texture loading
    void LoadPBRTextures(const std::string& albedoPath, 
                        const std::string& normalPath,
                        const std::string& metallicPath,
                        const std::string& roughnessPath,
                        const std::string& aoPath = "");

private:
    // OpenGL objects
    unsigned int VAO, VBO, EBO;
    
    // Setup functions
    void SetupMesh();
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
    // Texture loading
    unsigned int LoadTexture(const std::string& path, const aiScene* scene);
    unsigned int LoadTextureHDR(const std::string& path);
    
    // Utility
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene);
}; 