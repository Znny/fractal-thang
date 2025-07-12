#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

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
};

#endif

