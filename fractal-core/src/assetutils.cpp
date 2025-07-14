#include "assetutils.h"
#include <iostream>
#include <algorithm>
#include <fstream>

// Static member initialization
std::string AssetUtils::assetsRootPath;
bool AssetUtils::initialized = false;

// Simple file existence check
bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

void AssetUtils::initializeAssetsRoot() {
    if (initialized) return;
    
    // Start with current directory
    std::string currentDir = ".";
    
    // Look for the assets directory
    std::string assetsPath = currentDir + "/assets";
    
    // If assets directory doesn't exist in current directory, 
    // try looking in the parent directory (in case we're in fractal-core/)
    if (!fileExists(assetsPath + "/models/.gitkeep")) {
        assetsPath = "../assets";
    }
    
    // If still not found, try looking in the grandparent directory
    if (!fileExists(assetsPath + "/models/.gitkeep")) {
        assetsPath = "../../assets";
    }
    
    if (fileExists(assetsPath + "/models/.gitkeep")) {
        assetsRootPath = assetsPath;
        std::cout << "Assets root found at: " << assetsRootPath << std::endl;
    } else {
        // Fallback: assume assets is in current directory
        assetsRootPath = "./assets";
        std::cout << "Assets directory not found, using fallback: " << assetsRootPath << std::endl;
    }
    
    initialized = true;
}

std::string AssetUtils::getAssetsRoot() {
    initializeAssetsRoot();
    return assetsRootPath;
}

std::string AssetUtils::getModelsDir() {
    return getAssetsRoot() + "/models";
}

std::string AssetUtils::getTexturesDir() {
    return getAssetsRoot() + "/textures";
}

std::string AssetUtils::getShadersDir() {
    return getAssetsRoot() + "/shaders";
}

std::string AssetUtils::resolveAssetPath(const std::string& relativePath) {
    if (relativePath.empty()) {
        return "";
    }
    
    // If the path starts with / or contains : (Windows drive), treat as absolute
    if (relativePath[0] == '/' || relativePath.find(':') != std::string::npos) {
        return normalizePath(relativePath);
    }
    
    // Resolve relative to assets root
    std::string fullPath = getAssetsRoot() + "/" + relativePath;
    return normalizePath(fullPath);
}

std::string AssetUtils::resolveModelPath(const std::string& modelName) {
    if (modelName.empty()) {
        return "";
    }
    
    // If it's already a full path, return as-is
    if (modelName[0] == '/' || modelName.find(':') != std::string::npos) {
        return normalizePath(modelName);
    }
    
    // Resolve relative to models directory
    std::string fullPath = getModelsDir() + "/" + modelName;
    return normalizePath(fullPath);
}

std::string AssetUtils::resolveTexturePath(const std::string& textureName) {
    if (textureName.empty()) {
        return "";
    }
    std::cout << "Texture name: " << textureName << std::endl;

    // If it's already a full path, return as-is
    if (textureName[0] == '/' || textureName.find(':') != std::string::npos) {
        return normalizePath(textureName);
    }
    std::string cleanTextureName = normalizePath(textureName);
    std::cout << "Normalized Texture name: " << cleanTextureName << std::endl;
    
    // Remove "texture/" prefix if it exists (common in model relative paths)
    if (cleanTextureName.substr(0, 9) == "textures/") {
        cleanTextureName = cleanTextureName.substr(9);
    }
    std::cout << "Cleaned texture name: " << cleanTextureName << std::endl;
    
    // Resolve relative to textures directory
    std::string fullPath = getTexturesDir() + "/" + cleanTextureName;

    std::cout << "Resolved texture path: " << fullPath << std::endl;

    return normalizePath(fullPath);
}

std::string AssetUtils::resolveShaderPath(const std::string& shaderName) {
    if (shaderName.empty()) {
        return "";
    }
    
    // If it's already a full path, return as-is
    if (shaderName[0] == '/' || shaderName.find(':') != std::string::npos) {
        return normalizePath(shaderName);
    }
    
    // Resolve relative to shaders directory
    std::string fullPath = getShadersDir() + "/" + shaderName;
    return normalizePath(fullPath);
}

bool AssetUtils::fileExists(const std::string& path) {
    return ::fileExists(path);
}

std::string AssetUtils::getFileDirectory(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return filePath.substr(0, lastSlash);
    }
    return "";
}

std::string AssetUtils::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // Convert backslashes to forward slashes for consistency
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // Remove any trailing slashes
    while (!normalized.empty() && normalized.back() == '/') {
        normalized.pop_back();
    }
    
    return normalized;
} 