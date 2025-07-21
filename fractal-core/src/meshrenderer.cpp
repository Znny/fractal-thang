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
        for(unsigned int i = 0; i < (unsigned long)TextureType::MAX_TEXTURE_TYPES; i++) {
            if(m_mesh->textureIndex[i] != 0) {
                std::cout << "Texture " << TextureTypeToString((TextureType)i) << " loaded, index: " << m_mesh->textureIndex[i] << std::endl;
            }
        }
    }
    
    m_shaderProgram.Use();
    
    // Set view position
    glUniform3fv(glGetUniformLocation(m_shaderProgram.programId, "viewPos"), 1, glm::value_ptr(viewPos));
    
    // Set light uniforms
    SetLightUniforms();

    std::vector<const char*> textureNames = {
        "albedoMap",
        "normalMap",
        "metallicMap",
        "roughnessMap",
        "aoMap"
    };

    for(unsigned int i = 0; i < (unsigned long)TextureType::MAX_TEXTURE_TYPES; i++) {
        if(m_mesh->textureIndex[i] != 0) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_mesh->textureIndex[i]);
            glUniform1i(glGetUniformLocation(m_shaderProgram.programId, textureNames[i]), i);
        }
    }
    
    // Render each instance
    for (const auto& instance : m_instances) {
        // Set material uniforms
        SetMaterialUniforms(instance);
        
        // Set matrix uniforms
        SetMatrixUniforms(instance.transform, viewMatrix, projectionMatrix);
        
        

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