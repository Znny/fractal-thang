#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "trianglerenderer.h"
#include "window.h"

EMSCRIPTEN_BINDINGS(my_module) {
    
    // GLM Vector3 binding
    emscripten::class_<glm::vec3>("Vec3")
        .constructor<>()
        .constructor<float, float, float>();

    // GLM Matrix4 binding
    emscripten::class_<glm::mat4>("Mat4")
        .constructor<>()
        .constructor<float>();

    // GLM static functions
    emscripten::function("translate", static_cast<glm::mat4(*)(const glm::mat4&, const glm::vec3&)>(&glm::translate));
    emscripten::function("rotate", static_cast<glm::mat4(*)(const glm::mat4&, float, const glm::vec3&)>(&glm::rotate));
    emscripten::function("scale", static_cast<glm::mat4(*)(const glm::mat4&, const glm::vec3&)>(&glm::scale));
    emscripten::function("perspective", static_cast<glm::mat4(*)(float, float, float, float)>(&glm::perspective));
    emscripten::function("lookAt", static_cast<glm::mat4(*)(const glm::vec3&, const glm::vec3&, const glm::vec3&)>(&glm::lookAt));
    
    //Classes
    // Renderer class
    emscripten::class_<TriangleRenderer>("TriangleRenderer")
        .constructor<>()
        .function("render", &TriangleRenderer::Render)
        .function("setModelMatrix", &TriangleRenderer::SetModelMatrix)
        .function("setViewMatrix", &TriangleRenderer::SetViewMatrix)
        .function("setProjectionMatrix", &TriangleRenderer::SetProjectionMatrix);

    // Window class
    emscripten::class_<Window>("Window")
        .constructor<>()
        .function("init", &Window::Init)
        .function("destroy", &Window::Destroy)
        .function("resize", &Window::Resize)
        .function("swapBuffers", &Window::SwapBuffers)
        .function("pollEvents", &Window::PollEvents)
        .function("getWidth", &Window::GetWidth)
        .function("getHeight", &Window::GetHeight);
}










