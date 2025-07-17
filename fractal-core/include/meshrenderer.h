#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "mesh.h"
#include "light.h"
#include "shaderprogram.h"

struct MeshInstance {
    glm::mat4 transform;
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    
    MeshInstance() : transform(1.0f), albedo(0.5f, 0.0f, 0.5f), metallic(0.0f), roughness(0.5f), ao(1.0f) {}
};

class MeshRenderer {
public:
    MeshRenderer();
    ~MeshRenderer();

    // Mesh management
    void SetMesh(Mesh* mesh);
    void AddInstance(const MeshInstance& instance);
    void ClearInstances();
    
    // Transform management
    void SetTransform(long unsigned int instanceIndex, const glm::mat4& transform);
    void SetMaterial(long unsigned int instanceIndex, const glm::vec3& albedo, float metallic, float roughness, float ao);
    
    // Light management
    void SetLights(const std::vector<Light>& lights);
    void SetLight(int index, const Light& light);
    
    // Rendering
    void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos);
    
    // Shader management
    bool LoadShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void UseShader();

private:
    bool bFirstRender = true;
    // Shader program
    ShaderProgram m_shaderProgram;
    
    // Mesh and instances
    Mesh* m_mesh;
    std::vector<MeshInstance> m_instances;
    
    // Lights
    std::vector<Light> m_lights;
    
    // Uniform setters
    void SetMaterialUniforms(const MeshInstance& instance);
    void SetLightUniforms();
    void SetMatrixUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
}; 