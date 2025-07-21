
#include <iostream>

//todo: do this in a cleaner way
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/assimp/contrib/stb/stb_image.h"

#include "glreq.h"
#include "TextureManager.h"
#include "Texture.h"
#include "assetutils.h"


TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {
    if(instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

TextureManager::TextureManager() {
    if(instance == nullptr) {
        instance = this;
    }
    GenerateDefaultTexture();
}

TextureManager::~TextureManager() {
    if(instance == this) {
        instance = nullptr;
    }
}


std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& path, TextureType type) {
    //check if texture is already loaded
    std::shared_ptr<Texture> texture = GetTexture(path);
    if(texture != nullptr) {
        std::cout << "Texture already loaded: " << path << std::endl;
        return texture;
    }
    texture = std::make_shared<Texture>();
    texture->SetType(type);
    texture->SetPath(path);
    texture->SetName(path);

    std::string texturePath = AssetUtils::resolveTexturePath(path);

    TextureData textureData;

    textureData.pixels = stbi_load(texturePath.c_str(), (int*)&textureData.width, (int*)&textureData.height, (int*)&textureData.channels, 0);

    if(textureData.pixels == nullptr) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        return textures[0];
    }

    // Set the format based on number of channels
    if (textureData.channels == 4) {
        textureData.format = GL_RGBA;
    } else if (textureData.channels == 3) {
        textureData.format = GL_RGB;
    } else if (textureData.channels == 1) {
        textureData.format = GL_RED;
    } else {
        std::cerr << "Unsupported number of channels: " << textureData.channels << std::endl;
        stbi_image_free(textureData.pixels);
        return textures[0];
    }

    //std::cout << "Loading texture: " << path << std::endl;
    //std::cout << "  Dimensions: " << textureData.width << "x" << textureData.height << std::endl;
    //std::cout << "  Channels: " << textureData.channels << std::endl;
    //std::cout << "  Format: " << textureData.format << std::endl;

    texture->CreateTextureFromData(textureData);
    
    // Free the pixel data after uploading to GPU
    stbi_image_free(textureData.pixels);

    textures.push_back(texture);

    return texture;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& path) {
    // check if texture is already loaded
    for (auto& texture : textures) {
        if (texture->path == path) {
            return texture;
        }
    }
    return nullptr;
}

void TextureManager::GenerateDefaultTexture() {

    TextureData textureData;
    textureData.width = 128;
    textureData.height = 128;
    textureData.channels = 3;
    textureData.format = GL_RGB;
    textureData.pixels = new unsigned char[textureData.width * textureData.height * textureData.channels];

    const unsigned int checkerSize = 32;
    unsigned char* pixelData = static_cast<unsigned char*>(textureData.pixels);
    for(int s = 0; s < textureData.width; s++) 
    {
        for(int t = 0; t < textureData.height; t++) 
        {
            int index = (s * textureData.width + t) * textureData.channels;

            if((s / checkerSize) + (t / checkerSize) % 2 == 0) 
            {
                pixelData[index + 0] = 255;
                pixelData[index + 1] = 0;
                pixelData[index + 2] = 255;
            } 
            else 
            {
                pixelData[index + 0] = 0;
                pixelData[index + 1] = 0;
                pixelData[index + 2] = 0;
            }
        }
    }

    //set default texture parameters
    TextureParameters textureParameters;
    textureParameters.wrapMode_s = GL_REPEAT;
    textureParameters.wrapMode_t = GL_REPEAT;
    textureParameters.minFilter = GL_LINEAR_MIPMAP_LINEAR;
    textureParameters.magFilter = GL_LINEAR;
    textureParameters.maxLevel = 1;
    
    //generate the default texture
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->SetName("default");
    texture->SetPath("default");
    texture->SetType(TextureType::ALBEDO);
    texture->CreateTextureFromData(textureData, textureParameters);

    free(textureData.pixels);

    textures.push_back(texture);
}
    






