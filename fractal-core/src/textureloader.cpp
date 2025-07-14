#include "textureloader.h"
#include <iostream>
#include "glreq.h"
#include "assetutils.h"

// Include stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/assimp/contrib/stb/stb_image.h"

TextureLoader& TextureLoader::getInstance() {
    static TextureLoader instance;
    return instance;
}

TextureLoader::~TextureLoader() {
    clearCache();
}

unsigned int TextureLoader::loadTexture(const std::string& path) {
    // Check if texture is already loaded
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        std::cout << "Texture already loaded from cache: " << path << std::endl;
        return it->second;
    }
    
    // Resolve the texture path using AssetUtils
    std::string resolvedPath = AssetUtils::resolveTexturePath(path);
    std::cout << "Loading texture from file: " << resolvedPath << std::endl;
    
    // Load image using stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << resolvedPath << std::endl;
        std::cerr << "stb_image error: " << stbi_failure_reason() << std::endl;
        
        // Create a default texture and cache it
        unsigned int defaultTexture = createDefaultTexture(path);
        textureCache[path] = defaultTexture;
        return defaultTexture;
    }
    
    std::cout << "Loaded texture: " << width << "x" << height << " with " << channels << " channels" << std::endl;
    
    // Create OpenGL texture
    unsigned int textureID = createTextureFromData(data, width, height, channels);
    
    // Free the image data
    stbi_image_free(data);
    
    // Cache the texture
    textureCache[path] = textureID;
    
    std::cout << "Successfully loaded and cached texture with ID: " << textureID << std::endl;
    return textureID;
}

unsigned int TextureLoader::loadEmbeddedTexture(const std::string& path, const aiScene* scene) {
    // Check if texture is already loaded
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        std::cout << "Embedded texture already loaded from cache: " << path << std::endl;
        return it->second;
    }
    
    std::cout << "Loading embedded texture: " << path << std::endl;
    
    if (!scene) {
        std::cerr << "No scene provided for embedded texture loading" << std::endl;
        unsigned int defaultTexture = createDefaultTexture(path);
        textureCache[path] = defaultTexture;
        return defaultTexture;
    }
    
    // Search for the embedded texture in the scene
    for (unsigned int i = 0; i < scene->mNumTextures; i++) {
        aiTexture* embeddedTexture = scene->mTextures[i];
        std::cout << "Checking embedded texture " << i << ": " << embeddedTexture->mFilename.C_Str() << std::endl;
        
        // Check if this embedded texture matches our path
        std::string embeddedPath = embeddedTexture->mFilename.C_Str();
        if (embeddedPath.find(path) != std::string::npos || path.find(embeddedPath) != std::string::npos) {
            std::cout << "Found matching embedded texture!" << std::endl;
            
            unsigned int textureID = 0;
            
            if (embeddedTexture->mHeight == 0) {
                // Compressed texture (like PNG, JPG)
                std::cout << "Compressed texture detected, size: " << embeddedTexture->mWidth << " bytes" << std::endl;
                
                // Decode using stb_image
                int width, height, channels;
                unsigned char* data = stbi_load_from_memory(
                    reinterpret_cast<unsigned char*>(embeddedTexture->pcData), 
                    embeddedTexture->mWidth, 
                    &width, &height, &channels, 0
                );
                
                if (data) {
                    textureID = createTextureFromData(data, width, height, channels);
                    stbi_image_free(data);
                    std::cout << "Successfully decoded compressed embedded texture" << std::endl;
                } else {
                    std::cerr << "Failed to decode compressed embedded texture" << std::endl;
                    textureID = createDefaultTexture(path);
                }
            } else {
                // Uncompressed texture (raw pixel data)
                std::cout << "Uncompressed texture detected, size: " << embeddedTexture->mWidth << "x" << embeddedTexture->mHeight << std::endl;
                
                textureID = createTextureFromData(
                    reinterpret_cast<unsigned char*>(embeddedTexture->pcData), 
                    embeddedTexture->mWidth, 
                    embeddedTexture->mHeight, 
                    4  // Assimp stores as RGBA
                );
                std::cout << "Successfully loaded uncompressed embedded texture" << std::endl;
            }
            
            // Cache the texture
            textureCache[path] = textureID;
            return textureID;
        }
    }
    
    std::cout << "Embedded texture not found in scene: " << path << std::endl;
    unsigned int defaultTexture = createDefaultTexture(path);
    textureCache[path] = defaultTexture;
    return defaultTexture;
}

unsigned int TextureLoader::createDefaultTexture(const std::string& name) {
    // Check if default texture is already created
    auto it = textureCache.find(name);
    if (it != textureCache.end()) {
        return it->second;
    }
    
    std::cout << "Creating default texture in place of " << name << std::endl;
    
    // Create a magenta checkerboard pattern
    unsigned char defaultData[] = {
        255, 0, 255, 255,   // Magenta
        128, 128, 128, 255, // Gray
        128, 128, 128, 255, // Gray
        255, 0, 255, 255    // Magenta
    };
    
    unsigned int textureID = createTextureFromData(defaultData, 2, 2, 4);
    textureCache[name] = textureID;
    
    return textureID;
}

void TextureLoader::clearCache() {
    std::cout << "Clearing texture cache (" << textureCache.size() << " textures)" << std::endl;
    
    for (auto& pair : textureCache) {
        glDeleteTextures(1, &pair.second);
    }
    textureCache.clear();
}

bool TextureLoader::isTextureLoaded(const std::string& path) const {
    return textureCache.find(path) != textureCache.end();
}

unsigned int TextureLoader::getTextureID(const std::string& path) const {
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        return it->second;
    }
    return 0; // Return 0 if not found
}

unsigned int TextureLoader::createTextureFromData(unsigned char* data, int width, int height, int channels) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Determine format based on number of channels
    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    } else if (channels == 1) {
        format = GL_RED;
    }
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Set texture parameters
    setTextureParameters(textureID);
    
    return textureID;
}

void TextureLoader::setTextureParameters(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
} 