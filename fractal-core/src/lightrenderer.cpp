#include "lightrenderer.h"

LightRenderer::LightRenderer()
{
    Init();
}

LightRenderer::~LightRenderer()
{
}

void LightRenderer::Init()
{
    //Vertex should be changed to glm::vec3
    // Create sphere mesh
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    //generate a fibonacci sphere from a number of points
    const float radius = 0.3f;
    const float numPoints = 100;
    const float PI = 3.1415926f;
    const float PHI = 1.618033988749f; // Golden ratio
    
    for(int i = 0; i < numPoints; i++) {
        float z = 1.0f - (2.0f * i) / (numPoints - 1);
        float radius_at_y = sqrt(1.0f - z * z);
        
        float theta = 2.0f * PI * i / PHI;
        
        float x = radius_at_y * cos(theta);
        float y = radius_at_y * sin(theta);
        
        vertices.push_back(glm::vec3(radius * x, radius * y, radius * z));
    }

    numVertices = vertices.size();

    //load shaders
    shader = new ShaderProgram();
    shader->AttachShaderFromFile("light.vert", GL_VERTEX_SHADER);
    shader->AttachShaderFromFile("passthrough.frag", GL_FRAGMENT_SHADER);
    shader->Link();

    //create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    //set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    //unbind VAO
    glBindVertexArray(0);
}

void LightRenderer::Render(const std::vector<Light>& lights, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    //bind VAO
    glBindVertexArray(VAO);

    //bind shader
    shader->Use();

    //set up uniforms
    shader->SetUniformMat4("uView", viewMatrix);
    shader->SetUniformMat4("uProjection", projectionMatrix);

    glm::vec3 cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
    glm::vec3 cameraForward = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);

    //draw spheres
    for(const auto& light : lights) {
         // Create billboard matrix
         glm::mat4 billboardMatrix = glm::mat4(
            glm::vec4(cameraRight, 0.0f),
            glm::vec4(cameraUp, 0.0f),
            glm::vec4(cameraForward, 0.0f),
            glm::vec4(light.getPosition(), 1.0f)
         );

        shader->SetUniformMat4("uModel", billboardMatrix);
        shader->SetUniformVec3("uColor", light.getColor());
        shader->SetUniformFloat("uIntensity", light.getIntensity());

        glDrawArrays(GL_LINE_STRIP, 0, numVertices);
    }

    //unbind VAO
    glBindVertexArray(0);
}