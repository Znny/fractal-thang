#include "ShaderProgram.h"
#include <GLES3/gl3.h>

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
