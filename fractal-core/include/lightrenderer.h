#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H

#include "light.h"
#include "shaderprogram.h"
#include "glreq.h"

class LightRenderer
{
public:
    LightRenderer();
    ~LightRenderer();

    void Render(const std::vector<Light>& lights, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint numVertices;
    GLuint numIndices;

    ShaderProgram* shader;
    void Init();
};

#endif