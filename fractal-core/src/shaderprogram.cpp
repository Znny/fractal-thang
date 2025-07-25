// ShaderProgram.cpp

// C++ standard library
#include <fstream>
#include <sstream>
#include <iostream>

// local headers
#include "shaderprogram.h"
#include "assetutils.h"

ShaderProgram::ShaderProgram()
{
    programId = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programId);
}

void ShaderProgram::Use()
{
    glUseProgram(programId);
}

void ShaderProgram::AttachShaderFromFile(const char* path, unsigned int shaderType)
{
    // Resolve the shader path using AssetUtils
    std::string resolvedPath = AssetUtils::resolveShaderPath(path);
    
    std::ifstream file(resolvedPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << resolvedPath << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    const char* src = source.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader '" << path << "' compilation failed: " << infoLog << std::endl;
    }
    glAttachShader(programId, shader);
    glDeleteShader(shader);
}

void ShaderProgram::Link()
{
    glLinkProgram(programId);
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
    }
}

int ShaderProgram::GetAttribLocation(const char* name)
{
    return glGetAttribLocation(programId, name);
}

void ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4& value)
{
    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

void ShaderProgram::SetUniformVec3(const std::string& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location != -1) {
        glUniform3fv(location, 1, &value[0]);
    }
}

void ShaderProgram::SetUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}