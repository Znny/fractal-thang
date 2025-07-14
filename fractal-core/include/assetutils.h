#pragma once

#include <string>

class AssetUtils {
public:
    // Get the root assets directory path
    static std::string getAssetsRoot();
    
    // Get the models directory path
    static std::string getModelsDir();
    
    // Get the textures directory path
    static std::string getTexturesDir();
    
    // Get the shaders directory path
    static std::string getShadersDir();
    
    // Resolve a relative asset path to an absolute path
    // If the path is already absolute, return it as-is
    // If the path is relative, resolve it relative to the assets root
    static std::string resolveAssetPath(const std::string& relativePath);
    
    // Resolve a model path (relative to models directory)
    static std::string resolveModelPath(const std::string& modelName);
    
    // Resolve a texture path (relative to textures directory)
    static std::string resolveTexturePath(const std::string& textureName);
    
    // Resolve a shader path (relative to shaders directory)
    static std::string resolveShaderPath(const std::string& shaderName);
    
    // Check if a file exists
    static bool fileExists(const std::string& path);
    
    // Get the directory of a file
    static std::string getFileDirectory(const std::string& filePath);
    
    // Normalize path separators for the current platform
    static std::string normalizePath(const std::string& path);

private:
    // Initialize the assets root path (called once)
    static void initializeAssetsRoot();
    
    // Cached assets root path
    static std::string assetsRootPath;
    static bool initialized;
}; 