#pragma once

#include <glm/glm.hpp>

class Light {
public:
    enum class Type {
        Point,
        Directional,
        Spot
    };

    Light(Type type = Type::Point, 
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
        float intensity = 1.0f, 
        float range = 10.0f);

    ~Light() = default;

    // Transform
    void setPosition(const glm::vec3& position);
    void setDirection(const glm::vec3& direction);
    
    // Properties
    void setColor(const glm::vec3& color);
    void setIntensity(float intensity);
    void setRange(float range);
    void setType(Type type);
    
    // Getters
    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const;
    glm::vec3 getColor() const;
    float getIntensity() const;
    float getRange() const;
    Type getType() const;

private:
    Type m_type;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_color;
    float m_intensity;
    float m_range;
}; 