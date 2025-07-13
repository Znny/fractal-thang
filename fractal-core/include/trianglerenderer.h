#ifndef TRIANGLERENDERER_H
#define TRIANGLERENDERER_H

// C++ standard library
#include <string>

// glm header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// gl header
#include "glreq.h"

class TriangleRenderer
{
    public:
        TriangleRenderer();
        ~TriangleRenderer();

        void Render();

        void SetModelMatrix(const glm::mat4& modelMatrix);
        void SetViewMatrix(const glm::mat4& viewMatrix);
        void SetProjectionMatrix(const glm::mat4& projectionMatrix);

    private:
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
};

#endif 