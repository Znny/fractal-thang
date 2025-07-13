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
            // positions        // colors
             0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // red
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // green
             0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // blue
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        triangleSetup = true;
    }

    triangleShader->Use();
    triangleShader->SetUniformMat4("uModel", modelMatrix);
    triangleShader->SetUniformMat4("uView", viewMatrix);
    triangleShader->SetUniformMat4("uProjection", projectionMatrix);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
} 

void TriangleRenderer::SetModelMatrix(const glm::mat4& modelMatrix)
{
    this->modelMatrix = modelMatrix;
}

void TriangleRenderer::SetViewMatrix(const glm::mat4& viewMatrix)
{
    this->viewMatrix = viewMatrix;
}

void TriangleRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
}
