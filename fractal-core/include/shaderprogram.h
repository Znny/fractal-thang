#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

// c++ standard library
#include <string>

// glm header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// gl header
#include "glreq.h"

class ShaderProgram
{
    public:
        ShaderProgram();
        ~ShaderProgram();

        GLuint programId;

        void Use();
        void AttachShaderFromFile(const char* path, unsigned int shaderType);
        void Link();
        int GetAttribLocation(const char* name);
        void SetUniformMat4(const std::string& name, const glm::mat4& value);
};

#endif

