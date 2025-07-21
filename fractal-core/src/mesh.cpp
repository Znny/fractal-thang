#include <iostream>
#include <memory>

#include "assimp/material.h"


#include "glreq.h"
#include "Texture.h"
#include "assetutils.h"
#include "mesh.h"
#include "TextureManager.h"

Mesh::Mesh(const std::string& filename) 
    : albedo(1.0f, 0.0f, 1.0f), metallic(0.0f), roughness(0.5f), ao(1.0f) {
    
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
    unsigned int indexOffset = vertices.size();

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
            indices.push_back(face.mIndices[j] + indexOffset);
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
            
    const std::vector<aiTextureType> TextureTypeMappings[(unsigned long)TextureType::MAX_TEXTURE_TYPES] =
    {
        {aiTextureType_DIFFUSE, aiTextureType_BASE_COLOR}, // ALBEDO
        {aiTextureType_NORMALS, aiTextureType_NORMAL_CAMERA}, // NORMAL
        {aiTextureType_METALNESS}, // METALLIC
        {aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_SHININESS}, // ROUGHNESS
        {aiTextureType_LIGHTMAP, aiTextureType_AMBIENT_OCCLUSION} // AO
    };

    for(unsigned int i = 0; i < (unsigned long)TextureType::MAX_TEXTURE_TYPES; i++) 
    {
        for(aiTextureType type : TextureTypeMappings[i])
        {
            if(textureIndex[i] != 0)
            {
                continue;
            }

            if(material->GetTextureCount(type) > 0)
            {
                std::cout << "Loading " << TextureTypeToString((TextureType)i) << " texture of assimp type " << aiTextureTypeToString(type) << std::endl;
                aiString texturePath;
                material->GetTexture(type, 0, &texturePath);
                std::cout << "Texture path: " << texturePath.C_Str() << std::endl;
                std::shared_ptr<Texture> texture = TextureManager::GetInstance()->LoadTexture(texturePath.C_Str(), (TextureType)i);
                if (texture != nullptr) {
                    textureIndex[i] = texture->GetTextureId();
                } else {
                    std::cerr << "Failed to load texture: " << texturePath.C_Str() << std::endl;
                }
            }
        }
    }
}

void Mesh::SetupMesh() 
{
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

void Mesh::Draw() const 
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



