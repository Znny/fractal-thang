#include "Texture.h"

Texture::Texture() {
    this->type = TextureType::UNKNOWN;
}

Texture::~Texture() {
    glDeleteTextures(1, &this->data.id);
}

const char* TextureTypeToString(TextureType type) {
    switch(type) {
        case TextureType::UNKNOWN: return "UNKNOWN";
        case TextureType::ALBEDO: return "ALBEDO";
        case TextureType::NORMAL: return "NORMAL";
        case TextureType::METALLIC: return "METALLIC";
        case TextureType::ROUGHNESS: return "ROUGHNESS";
        case TextureType::AO: return "AO";
        default: return "UNKNOWN";
    }
}

void Texture::CreateTextureFromData(const TextureData& data, const TextureParameters& parameters) {

    this->data = data;
    this->parameters = parameters;

    glGenTextures(1, &this->data.id);
    glBindTexture(GL_TEXTURE_2D, this->data.id);
    
    // Set the format based on number of channels
    if (this->data.channels == 4) {
        this->data.format = GL_RGBA;
    } else if (this->data.channels == 3) {
        this->data.format = GL_RGB;
    } else if (this->data.channels == 1) {
        this->data.format = GL_RED;
    } else {
        std::cerr << "Unsupported number of channels: " << this->data.channels << std::endl;
    }
    
   glTexImage2D(GL_TEXTURE_2D, 
    0, 
    this->data.format, 
    this->data.width, 
    this->data.height, 
    0, 
    this->data.format, 
    GL_UNSIGNED_BYTE, this->data.pixels);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

   // Set texture parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->parameters.wrapMode_s);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->parameters.wrapMode_t);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->parameters.minFilter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->parameters.magFilter);

   // Generate mipmaps
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, this->parameters.maxLevel);
   glGenerateMipmap(GL_TEXTURE_2D);
}