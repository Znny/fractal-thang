// trianglerenderer.cpp

// C++ standard library
#include <iostream>
#include <string>
#include <memory>

// local headers
#include "trianglerenderer.h"
#include "shaderprogram.h"

namespace {
ShaderProgram* triangleShader = nullptr;
unsigned int VAO = 0, VBO = 0;
bool triangleSetup = false;
}

TriangleRenderer::TriangleRenderer()
{
}

TriangleRenderer::~TriangleRenderer()
{
}

void TriangleRenderer::Render()
{
    // --- Triangle rendering ---
    if (!triangleSetup) {
        triangleShader = new ShaderProgram();

        #ifdef __EMSCRIPTEN__
        triangleShader->AttachShaderFromFile("/shaders/passthrough.vert", GL_VERTEX_SHADER);
        triangleShader->AttachShaderFromFile("/shaders/passthrough.frag", GL_FRAGMENT_SHADER);
        triangleShader->Link();
        #else
        triangleShader->AttachShaderFromFile("fractal-core/shaders/passthrough.vert", GL_VERTEX_SHADER);
        triangleShader->AttachShaderFromFile("fractal-core/shaders/passthrough.frag", GL_FRAGMENT_SHADER);
        triangleShader->Link();
        #endif

        float vertices[] = {
            // positions      // texcoords
            0.0f,  0.5f, 0.0f,  0.5f, 1.0f,
           -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texcoord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        triangleSetup = true;
    }

    triangleShader->Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
} 