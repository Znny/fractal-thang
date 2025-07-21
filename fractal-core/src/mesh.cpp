#include "mesh.h"
#include <iostream>
#include "assimp/material.h"
#include "glreq.h"
#include "textureloader.h"
#include "assetutils.h"

Mesh::Mesh(const std::string& filename) 
    : albedoTexture(0), normalTexture(0), metallicTexture(0), roughnessTexture(0), aoTexture(0),
      albedo(1.0f, 0.0f, 1.0f), metallic(0.0f), roughness(0.5f), ao(1.0f) {
    
    // Resolve the full path using AssetUtils
    std::string filepath = AssetUtils::resolveModelPath(filename);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | 
        aiProcess_GenNormals | 
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    
    ProcessNode(scene->mRootNode, scene);
    SetupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene) {
    // Process all meshes in the node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }
    
    // Process children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    // Store the current number of vertices as the offset
    unsigned int vertexOffset = vertices.size();

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        
        // Position
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        
        // Normal
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        
        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        
        // Tangent
        if (mesh->HasTangentsAndBitangents()) {
            vertex.tangent = glm::vec3(
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            );
            
            vertex.bitangent = glm::vec3(
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            );
        }
        
        vertices.push_back(vertex);
    }
    
    // Process indices, offsetting by vertexOffset
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j] + vertexOffset);
        }
    }
    
    // Process materials
    std::cout << "Processing material " << mesh->mMaterialIndex << std::endl;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::cout << "Material name: " << material->GetName().C_Str() << std::endl;

    //debug print all texture types and their paths
    /*
    aiTextureType firstTextureType = aiTextureType_NONE;
    aiTextureType lastTextureType = AI_TEXTURE_TYPE_MAX;
    for(aiTextureType type = firstTextureType; type <= lastTextureType; type = aiTextureType(type + 1)) {
        std::cout << "Texture count for " << aiTextureTypeToString(type) << ": " << material->GetTextureCount(type) << std::endl;
        for(int i = 0; i < material->GetTextureCount(type); i++) {
            aiString str;
            material->GetTexture(type, i, &str);
            std::cout << "Texture path: " << str.C_Str() << std::endl;
        }
    }
    */        

    // Load PBR textures from material
    LoadMaterialTextures(material, aiTextureType_DIFFUSE, "albedo", scene);
    if(albedoTexture == 0) {
        LoadMaterialTextures(material, aiTextureType_BASE_COLOR, "albedo", scene);
    }

    LoadMaterialTextures(material, aiTextureType_NORMALS, "normal", scene);
    if(normalTexture == 0) {
        LoadMaterialTextures(material, aiTextureType_NORMAL_CAMERA, "normal", scene);
    }

    LoadMaterialTextures(material, aiTextureType_METALNESS, "metallic", scene);

    LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "roughness", scene);
    if(roughnessTexture == 0) {
        LoadMaterialTextures(material, aiTextureType_SHININESS, "roughness", scene);
    }

    LoadMaterialTextures(material, aiTextureType_LIGHTMAP, "ao", scene);
    if(aoTexture == 0) {
        LoadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "ao", scene);
    }
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    
    // Tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    
    // Bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    
    glBindVertexArray(0);
}

void Mesh::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::LoadPBRTextures(const std::string& albedoPath, 
                          const std::string& normalPath,
                          const std::string& metallicPath,
                          const std::string& roughnessPath,
                          const std::string& aoPath) {
    
    // This method is kept for backward compatibility but textures are now loaded
    // automatically from Assimp materials during mesh processing
    if (!albedoPath.empty()) albedoTexture = LoadTexture(albedoPath, nullptr); // Pass nullptr for now
    if (!normalPath.empty()) normalTexture = LoadTexture(normalPath, nullptr); // Pass nullptr for now
    if (!metallicPath.empty()) metallicTexture = LoadTexture(metallicPath, nullptr); // Pass nullptr for now
    if (!roughnessPath.empty()) roughnessTexture = LoadTexture(roughnessPath, nullptr); // Pass nullptr for now
    if (!aoPath.empty()) aoTexture = LoadTexture(aoPath, nullptr); // Pass nullptr for now
}

unsigned int Mesh::LoadTexture(const std::string& path, const aiScene* scene) {
    // Check if this is an embedded texture path (contains .fbm or similar)
    if (path.find(".fbm") != std::string::npos) {
        return TextureLoader::getInstance().loadEmbeddedTexture(path, scene);
    }
    
    // Check if this is an embedded texture (starts with '*')
    if (path[0] == '*') {
        return TextureLoader::getInstance().createDefaultTexture(path);
    }
    
    // For external textures, use the texture loader with AssetUtils path resolution
    return TextureLoader::getInstance().loadTexture(path);
}

std::vector<Texture> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const aiScene* scene) {
    std::vector<Texture> textures;
    
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        // Check if texture was already loaded
        bool skip = false;
        for (unsigned int j = 0; j < textures.size(); j++) {
            if (std::strcmp(textures[j].path.c_str(), str.C_Str()) == 0) {
                textures.push_back(textures[j]);
                skip = true;
                std::cout << "Skipping duplicate texture: " << str.C_Str() << std::endl;
                break;
            }
        }
        
        if (!skip) {
            Texture texture;
            texture.id = LoadTexture(str.C_Str(), scene); // Pass the scene for embedded texture loading
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            
            std::cout << "Loaded " << typeName << " texture with ID: " << texture.id << std::endl;
            
            // Assign to appropriate PBR texture slot
            if (typeName == "albedo") albedoTexture = texture.id;
            else if (typeName == "normal") normalTexture = texture.id;
            else if (typeName == "metallic") metallicTexture = texture.id;
            else if (typeName == "roughness") roughnessTexture = texture.id;
            else if (typeName == "ao") aoTexture = texture.id;
        }
    }
    return textures;
} 