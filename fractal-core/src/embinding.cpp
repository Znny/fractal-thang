// emscripten headers
#include <emscripten/bind.h>
#include <emscripten/val.h>

// local headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// local headers
#include "camera.h"
#include "trianglerenderer.h"
#include "window.h"
#include "debug-utils.h"

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
    
    // Camera class
    emscripten::class_<Camera>("Camera")
        .constructor<>()
        // Transform methods
        .function("setPosition", &Camera::setPosition)
        .function("setRotation", &Camera::setRotation)
        .function("setScale", &Camera::setScale)
        .function("getPosition", &Camera::getPosition)
        .function("getRotation", &Camera::getRotation)
        .function("getScale", &Camera::getScale)
        // First-person movement
        .function("moveForward", &Camera::moveForward)
        .function("moveRight", &Camera::moveRight)
        .function("moveUp", &Camera::moveUp)
        .function("moveLocal", &Camera::moveLocal)
        // First-person rotation
        .function("rotateYaw", &Camera::rotateYaw)
        .function("rotatePitch", &Camera::rotatePitch)
        .function("rotateRoll", &Camera::rotateRoll)
        // Matrix methods
        .function("getViewMatrix", &Camera::getViewMatrix)
        .function("getProjectionMatrix", &Camera::getProjectionMatrix)
        .function("getTransformMatrix", &Camera::getTransformMatrix)
        // Projection methods
        .function("setPerspective", &Camera::setPerspective)
        .function("setOrthographic", &Camera::setOrthographic)
        .function("getFOV", &Camera::getFOV)
        .function("getAspect", &Camera::getAspect)
        .function("getNear", &Camera::getNear)
        .function("getFar", &Camera::getFar)
        // Local direction methods
        .function("getLocalForward", &Camera::getLocalForward)
        .function("getLocalRight", &Camera::getLocalRight)
        .function("getLocalUp", &Camera::getLocalUp);

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

    // Debug utils
    emscripten::function("printMatrix", &printMatrix);
}











