#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

class ShaderProgram
{
    public:
        ShaderProgram();
        ~ShaderProgram();

        unsigned int programId;

        void Use();
};

#endif

