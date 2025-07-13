#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

// World reference vectors
const glm::vec3 Camera::WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::WORLD_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Camera::WORLD_FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);

Camera::Camera() 
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(1.0f, 0.0f, 0.0f, 0.0f) // Identity quaternion
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_fov(45.0f)
    , m_aspect(16.0f / 9.0f)
    , m_near(0.1f)
    , m_far(100.0f)
    , m_isPerspective(true)
{
}

// Transform methods
void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
}

void Camera::setRotation(const glm::vec3& eulerAngles) {
    // Convert euler angles (in radians) to quaternion
    m_rotation = glm::quat(eulerAngles);
}

void Camera::setScale(const glm::vec3& scale) {
    m_scale = scale;
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}

glm::vec3 Camera::getRotation() const {
    // Convert quaternion back to euler angles
    return glm::eulerAngles(m_rotation);
}

glm::vec3 Camera::getScale() const {
    return m_scale;
}

// First-person movement methods
void Camera::moveForward(float distance) {
    m_position += getLocalForward() * distance;
}

void Camera::moveRight(float distance) {
    m_position += getLocalRight() * distance;
}

void Camera::moveUp(float distance) {
    m_position += getLocalUp() * distance;
}

void Camera::moveLocal(const glm::vec3& direction, float distance) {
    m_position += direction * distance;
}

// First-person rotation methods
void Camera::rotateYaw(float angle) {
    // Rotate around world up vector
    glm::quat yawRotation = glm::angleAxis(angle, WORLD_UP);
    m_rotation = yawRotation * m_rotation;
}

void Camera::rotatePitch(float angle) {
    // Rotate around local right vector
    glm::quat pitchRotation = glm::angleAxis(angle, getLocalRight());
    m_rotation = pitchRotation * m_rotation;
}

void Camera::rotateRoll(float angle) {
    // Rotate around local forward vector
    glm::quat rollRotation = glm::angleAxis(angle, getLocalForward());
    m_rotation = rollRotation * m_rotation;
}

// Matrix methods
glm::mat4 Camera::getViewMatrix() const {
    // For a camera, the view matrix is the inverse of the transform matrix
    return glm::inverse(getTransformMatrix());
}

glm::mat4 Camera::getProjectionMatrix() const {
    if (m_isPerspective) {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    } else {
        // For orthographic, we need to calculate bounds based on aspect ratio
        float height = 2.0f * m_near * std::tan(glm::radians(m_fov) * 0.5f);
        float width = height * m_aspect;
        return glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, m_near, m_far);
    }
}

glm::mat4 Camera::getTransformMatrix() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_position);
    transform = transform * glm::mat4_cast(m_rotation);
    transform = glm::scale(transform, m_scale);
    return transform;
}

// Projection methods
void Camera::setPerspective(float fov, float aspect, float near, float far) {
    m_fov = fov;
    m_aspect = aspect;
    m_near = near;
    m_far = far;
    m_isPerspective = true;
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
    m_near = near;
    m_far = far;
    m_isPerspective = false;
    // Note: For orthographic, we'd need to store the bounds, but for simplicity
    // we'll use the perspective parameters to calculate orthographic bounds
}

float Camera::getFOV() const {
    return m_fov;
}

float Camera::getAspect() const {
    return m_aspect;
}

float Camera::getNear() const {
    return m_near;
}

float Camera::getFar() const {
    return m_far;
}

// Local direction methods
glm::vec3 Camera::getLocalForward() const {
    return glm::normalize(m_rotation * WORLD_FORWARD);
}

glm::vec3 Camera::getLocalRight() const {
    return glm::normalize(m_rotation * WORLD_RIGHT);
}

glm::vec3 Camera::getLocalUp() const {
    return glm::normalize(m_rotation * WORLD_UP);
}

void Camera::updateTransform() {
    // This method is called when we need to update the transform
    // In our current implementation, we calculate matrices on-demand
    // so this method is not strictly necessary, but kept for consistency
} 