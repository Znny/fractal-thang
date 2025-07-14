#include "assetutils.h"
#include <iostream>

int main() {
    std::cout << "=== AssetUtils Integration Test ===" << std::endl;
    
    // Test directory resolution
    std::cout << "\n1. Directory Resolution:" << std::endl;
    std::cout << "Assets root: " << AssetUtils::getAssetsRoot() << std::endl;
    std::cout << "Models dir: " << AssetUtils::getModelsDir() << std::endl;
    std::cout << "Textures dir: " << AssetUtils::getTexturesDir() << std::endl;
    std::cout << "Shaders dir: " << AssetUtils::getShadersDir() << std::endl;
    
    // Test path resolution
    std::cout << "\n2. Path Resolution:" << std::endl;
    std::cout << "Model 'columns.fbx': " << AssetUtils::resolveModelPath("columns.fbx") << std::endl;
    std::cout << "Texture 'Marble1_bcolor.png': " << AssetUtils::resolveTexturePath("Marble1_bcolor.png") << std::endl;
    std::cout << "Shader 'pbr.vert': " << AssetUtils::resolveShaderPath("pbr.vert") << std::endl;
    
    // Test file existence
    std::cout << "\n3. File Existence:" << std::endl;
    std::cout << "Models dir exists: " << (AssetUtils::fileExists(AssetUtils::getModelsDir()) ? "Yes" : "No") << std::endl;
    std::cout << "Textures dir exists: " << (AssetUtils::fileExists(AssetUtils::getTexturesDir()) ? "Yes" : "No") << std::endl;
    std::cout << "Shaders dir exists: " << (AssetUtils::fileExists(AssetUtils::getShadersDir()) ? "Yes" : "No") << std::endl;
    
    // Test specific files
    std::cout << "\n4. Specific File Tests:" << std::endl;
    std::string modelPath = AssetUtils::resolveModelPath("columns.fbx");
    std::string texturePath = AssetUtils::resolveTexturePath("Marble1_bcolor.png");
    std::string shaderPath = AssetUtils::resolveShaderPath("pbr.vert");
    
    std::cout << "Model file exists: " << (AssetUtils::fileExists(modelPath) ? "Yes" : "No") << std::endl;
    std::cout << "Texture file exists: " << (AssetUtils::fileExists(texturePath) ? "Yes" : "No") << std::endl;
    std::cout << "Shader file exists: " << (AssetUtils::fileExists(shaderPath) ? "Yes" : "No") << std::endl;
    
    // Test path normalization
    std::cout << "\n5. Path Normalization:" << std::endl;
    std::cout << "Normalized 'models\\test.fbx': " << AssetUtils::normalizePath("models\\test.fbx") << std::endl;
    std::cout << "Normalized 'textures/test.png': " << AssetUtils::normalizePath("textures/test.png") << std::endl;
    
    std::cout << "\n=== Test Complete ===" << std::endl;
    return 0;
} 