#include "assetutils.h"
#include <iostream>

int main() {
    std::cout << "Testing AssetUtils..." << std::endl;
    
    std::cout << "Assets root: " << AssetUtils::getAssetsRoot() << std::endl;
    std::cout << "Models dir: " << AssetUtils::getModelsDir() << std::endl;
    std::cout << "Textures dir: " << AssetUtils::getTexturesDir() << std::endl;
    std::cout << "Shaders dir: " << AssetUtils::getShadersDir() << std::endl;
    
    // Test path resolution
    std::cout << "\nPath resolution tests:" << std::endl;
    std::cout << "Model 'columns.fbx': " << AssetUtils::resolveModelPath("columns.fbx") << std::endl;
    std::cout << "Texture 'Marble1_bcolor.png': " << AssetUtils::resolveTexturePath("Marble1_bcolor.png") << std::endl;
    std::cout << "Shader 'pbr.vert': " << AssetUtils::resolveShaderPath("pbr.vert") << std::endl;
    
    // Test file existence
    std::cout << "\nFile existence tests:" << std::endl;
    std::cout << "Models dir exists: " << (AssetUtils::fileExists(AssetUtils::getModelsDir()) ? "Yes" : "No") << std::endl;
    std::cout << "Textures dir exists: " << (AssetUtils::fileExists(AssetUtils::getTexturesDir()) ? "Yes" : "No") << std::endl;
    std::cout << "Shaders dir exists: " << (AssetUtils::fileExists(AssetUtils::getShadersDir()) ? "Yes" : "No") << std::endl;
    
    return 0;
} 