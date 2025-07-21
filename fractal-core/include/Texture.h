#pragma once

// stl
#include <iostream>
#include <string>

// gl
#include "glreq.h"


// texture types
enum class TextureType {
    UNKNOWN = -1,
    ALBEDO,
    NORMAL,
    METALLIC,
    ROUGHNESS,
    AO,
    MAX_TEXTURE_TYPES
};

const char* TextureTypeToString(TextureType type);

struct TextureData {
    GLuint  id = GL_INVALID_INDEX;
    GLuint  width = 0;
    GLuint  height = 0;
    GLuint  channels = 0;
    GLenum  format = GL_INVALID_ENUM;
    GLvoid* pixels = nullptr;

    void Print() const {
        std::cout << "TextureData: " << std::endl;
        std::cout << "  id: " << id << std::endl;
        std::cout << "  width: " << width << std::endl;
        std::cout << "  height: " << height << std::endl;
        std::cout << "  channels: " << channels << std::endl;
    }
};

struct TextureParameters {
    TextureParameters() {
        this->wrapMode_s = GL_REPEAT;
        this->wrapMode_t = GL_REPEAT;
        this->minFilter = GL_LINEAR_MIPMAP_LINEAR;
        this->magFilter = GL_LINEAR;
        this->maxLevel = 4; 
    }

    GLenum wrapMode_s = GL_INVALID_ENUM;
    GLenum wrapMode_t = GL_INVALID_ENUM;
    GLenum minFilter = GL_INVALID_ENUM;
    GLenum magFilter = GL_INVALID_ENUM;
    GLenum maxLevel = GL_INVALID_ENUM; //number of mipmaps

    void Print() const {
        std::cout << "TextureParameters: " << std::endl;
        std::cout << "  wrapMode_s: " << wrapMode_s << std::endl;
        std::cout << "  wrapMode_t: " << wrapMode_t << std::endl;
        std::cout << "  minFilter: " << minFilter << std::endl;
        std::cout << "  magFilter: " << magFilter << std::endl;
        std::cout << "  maxLevel: " << maxLevel << std::endl;
    }
};

class Texture {
    friend class TextureManager;
public:
    Texture();
    ~Texture();

    GLuint GetTextureId() const { return data.id; };
    const TextureType& GetType() const { return type; };
    const TextureData& GetData() const { return data; };
    const TextureParameters& GetParameters() const { return parameters; };
    const std::string& GetName() const { return name; };
    const std::string& GetPath() const { return path; };

    void SetType(TextureType type) { this->type = type; };
    void SetData(TextureData data) { this->data = data; };
    void SetParameters(TextureParameters parameters) { this->parameters = parameters; };
    void SetName(std::string name) { this->name = name; };
    void SetPath(std::string path) { this->path = path; };

    void Print() const {
        std::cout << "Texture: " << std::endl;
        std::cout << "  name: " << name << std::endl;
        std::cout << "  path: " << path << std::endl;
        std::cout << "  type: " << TextureTypeToString(type) << std::endl;
        data.Print();
        parameters.Print();
    }

protected:
    TextureType type;
    TextureData data;
    TextureParameters parameters;

    std::string name;
    std::string path;

private:
    void CreateTextureFromData(const TextureData& data, const TextureParameters& parameters = TextureParameters());
};
