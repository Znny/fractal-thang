#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <vector>
#include <cmath>

class Renderer {
private:
    std::vector<float> vertices;
    int width;
    int height;

public:
    Renderer(int w, int h) : width(w), height(h) {
        // Initialize with a basic triangle
        vertices = {
            0.0f, 0.5f, 0.0f,    // top
            -0.5f, -0.5f, 0.0f,  // bottom left
            0.5f, -0.5f, 0.0f    // bottom right
        };
    }

    // Get vertices as a Float32Array for WebGL
    emscripten::val getVertices() {
        return emscripten::val(emscripten::typed_memory_view(vertices.size(), vertices.data()));
    }

    // Update vertices for fractal generation
    void updateVertices(const std::vector<float>& newVertices) {
        vertices = newVertices;
    }

    // Get the number of vertices
    int getVertexCount() const {
        return vertices.size() / 3;
    }
};

// Emscripten bindings
EMSCRIPTEN_BINDINGS(Renderer) {
    emscripten::class_<Renderer>("Renderer")
        .constructor<int, int>()
        .function("getVertices", &Renderer::getVertices)
        .function("updateVertices", &Renderer::updateVertices)
        .function("getVertexCount", &Renderer::getVertexCount);
} 