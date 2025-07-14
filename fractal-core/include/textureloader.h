#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <assimp/scene.h>

class TextureLoader {
public:
    static TextureLoader& getInstance();
    
    // Load texture from file path
    unsigned int loadTexture(const std::string& path);
    
    // Load embedded texture from Assimp scene
    unsigned int loadEmbeddedTexture(const std::string& path, const aiScene* scene);
    
    // Create a default texture (for missing textures)
    unsigned int createDefaultTexture(const std::string& name = "default");
    
    // Clear all cached textures
    void clearCache();
    
    // Get texture info
    bool isTextureLoaded(const std::string& path) const;
    unsigned int getTextureID(const std::string& path) const;

private:
    TextureLoader() = default;
    ~TextureLoader();
    
    // Prevent copying
    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
    
    // Cache of loaded textures
    std::unordered_map<std::string, unsigned int> textureCache;
    
    // Helper functions
    unsigned int createTextureFromData(unsigned char* data, int width, int height, int channels);
    void setTextureParameters(unsigned int textureID);
}; 