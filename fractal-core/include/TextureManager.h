#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Texture.h"

class TextureManager {
public:
    static TextureManager* GetInstance();

    TextureManager();
    ~TextureManager();

    std::shared_ptr<Texture> LoadTexture(const std::string& path, TextureType type);
    std::shared_ptr<Texture> GetTexture(const std::string& path);
    void PrintTextures() const;

private:
    static TextureManager* instance;

    void GenerateDefaultTexture();

    std::vector<std::shared_ptr<Texture>> textures;
};
