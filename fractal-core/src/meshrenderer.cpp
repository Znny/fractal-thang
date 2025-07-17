#include "meshrenderer.h"
#include <iostream>
#include "glreq.h"
#include <glm/gtc/type_ptr.hpp>

MeshRenderer::MeshRenderer() {
    // Initialize with default lights
    m_lights.resize(4);
    for (int i = 0; i < 4; i++) {
        m_lights[i] = Light();
    }
}

MeshRenderer::~MeshRenderer() {
    // ShaderProgram destructor handles cleanup
}

void MeshRenderer::SetMesh(Mesh* mesh) {
    m_mesh = mesh;
}

void MeshRenderer::AddInstance(const MeshInstance& instance) {
    m_instances.push_back(instance);
}

void MeshRenderer::ClearInstances() {
    m_instances.clear();
}

void MeshRenderer::SetTransform(long unsigned int instanceIndex, const glm::mat4& transform) {
    if (instanceIndex < m_instances.size()) {
        m_instances[instanceIndex].transform = transform;
    }
}

void MeshRenderer::SetMaterial(long unsigned int instanceIndex, const glm::vec3& albedo, float metallic, float roughness, float ao) {
    if (instanceIndex < m_instances.size()) {
        m_instances[instanceIndex].albedo = albedo;
        m_instances[instanceIndex].metallic = metallic;
        m_instances[instanceIndex].roughness = roughness;
        m_instances[instanceIndex].ao = ao;
    }
}

void MeshRenderer::SetLights(const std::vector<Light>& lights) {
    m_lights = lights;
    if (m_lights.size() != 4) {
        m_lights.resize(4);
    }
}

void MeshRenderer::SetLight(int index, const Light& light) {
    if (index >= 0 && index < 4) {
        m_lights[index] = light;
    }
}

void MeshRenderer::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos) {
    if (!m_mesh) return;

    if(bFirstRender) {
        bFirstRender = false;
        std::cout << "First render" << std::endl;
        if(m_mesh->albedoTexture) {
            std::cout << "Albedo texture: " << m_mesh->albedoTexture << std::endl;
        }
        if(m_mesh->normalTexture) {
            std::cout << "Normal texture: " << m_mesh->normalTexture << std::endl;
        }
        if(m_mesh->metallicTexture) {
            std::cout << "Metallic texture: " << m_mesh->metallicTexture << std::endl;
        }
        if(m_mesh->roughnessTexture) {
            std::cout << "Roughness texture: " << m_mesh->roughnessTexture << std::endl;
        }
        if(m_mesh->aoTexture) { 
            std::cout << "AO texture: " << m_mesh->aoTexture << std::endl;
        }
    }
    
    m_shaderProgram.Use();
    
    // Set view position
    glUniform3fv(glGetUniformLocation(m_shaderProgram.programId, "viewPos"), 1, glm::value_ptr(viewPos));
    
    // Set light uniforms
    SetLightUniforms();
    
    // Render each instance
    for (const auto& instance : m_instances) {
        // Set material uniforms
        SetMaterialUniforms(instance);
        
        // Set matrix uniforms
        SetMatrixUniforms(instance.transform, viewMatrix, projectionMatrix);
        
        // Bind textures
        if (m_mesh->albedoTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_mesh->albedoTexture);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, "albedoMap"), 0);
        }
        
        if (m_mesh->normalTexture) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_mesh->normalTexture);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, "normalMap"), 1);
        }
        
        if (m_mesh->metallicTexture) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_mesh->metallicTexture);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, "metallicMap"), 2);
        }
        
        if (m_mesh->roughnessTexture) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_mesh->roughnessTexture);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, "roughnessMap"), 3);
        }
        
        if (m_mesh->aoTexture) {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_mesh->aoTexture);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, "aoMap"), 4);
        }
        
        // Draw mesh
        m_mesh->Draw();
    }
}

bool MeshRenderer::LoadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    try {
        m_shaderProgram.AttachShaderFromFile(vertexPath.c_str(), GL_VERTEX_SHADER);
        m_shaderProgram.AttachShaderFromFile(fragmentPath.c_str(), GL_FRAGMENT_SHADER);
        m_shaderProgram.Link();
        return true;
    } catch (...) {
        return false;
    }
}

void MeshRenderer::UseShader() {
    m_shaderProgram.Use();
}

void MeshRenderer::SetMaterialUniforms(const MeshInstance& instance) {
    glUniform3fv(glGetUniformLocation(m_shaderProgram.programId, "albedo"), 1, glm::value_ptr(instance.albedo));
    glUniform1f(glGetUniformLocation(m_shaderProgram.programId, "metallic"), instance.metallic);
    glUniform1f(glGetUniformLocation(m_shaderProgram.programId, "roughness"), instance.roughness);
    glUniform1f(glGetUniformLocation(m_shaderProgram.programId, "ao"), instance.ao);
}

void MeshRenderer::SetLightUniforms() {
    for (int i = 0; i < 4; i++) {
        std::string posName = "lightPositions[" + std::to_string(i) + "]";
        std::string colorName = "lightColors[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(m_shaderProgram.programId, posName.c_str()), 1, 
                    glm::value_ptr(m_lights[i].getPosition()));
        glUniform3fv(glGetUniformLocation(m_shaderProgram.programId, colorName.c_str()), 1, 
                    glm::value_ptr(m_lights[i].getColor() * m_lights[i].getIntensity()));
    }
}

void MeshRenderer::SetMatrixUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
} 