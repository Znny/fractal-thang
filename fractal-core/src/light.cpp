#include "light.h"

Light::Light(Type type, glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, float range) 
    : m_type(type), m_position(position), m_direction(direction),
      m_color(color), m_intensity(intensity), m_range(range) {
}

void Light::setPosition(const glm::vec3& position) {
    m_position = position;
}

void Light::setDirection(const glm::vec3& direction) {
    m_direction = glm::normalize(direction);
}

void Light::setColor(const glm::vec3& color) {
    m_color = color;
}

void Light::setIntensity(float intensity) {
    m_intensity = intensity;
}

void Light::setRange(float range) {
    m_range = range;
}

void Light::setType(Type type) {
    m_type = type;
}

glm::vec3 Light::getPosition() const {
    return m_position;
}

glm::vec3 Light::getDirection() const {
    return m_direction;
}

glm::vec3 Light::getColor() const {
    return m_color;
}

float Light::getIntensity() const {
    return m_intensity;
}

float Light::getRange() const {
    return m_range;
}

Light::Type Light::getType() const {
    return m_type;
} 