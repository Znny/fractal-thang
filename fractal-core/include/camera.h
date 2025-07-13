#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera {
public:
    Camera();
    ~Camera() = default;

    // Transform
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& eulerAngles);
    void setScale(const glm::vec3& scale);
    
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

    // First-person movement
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveLocal(const glm::vec3& direction, float distance);

    // First-person rotation
    void rotateYaw(float angle);
    void rotatePitch(float angle);
    void rotateRoll(float angle);

    // Matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getTransformMatrix() const;

    // Projection
    void setPerspective(float fov, float aspect, float near, float far);
    void setOrthographic(float left, float right, float bottom, float top, float near, float far);
    
    float getFOV() const;
    float getAspect() const;
    float getNear() const;
    float getFar() const;

    // Local directions
    glm::vec3 getLocalForward() const;
    glm::vec3 getLocalRight() const;
    glm::vec3 getLocalUp() const;

private:
    void updateTransform();

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    
    // Projection parameters
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
    bool m_isPerspective;

    // World reference vectors
    static const glm::vec3 WORLD_UP;
    static const glm::vec3 WORLD_RIGHT;
    static const glm::vec3 WORLD_FORWARD;
}; 