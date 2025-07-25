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
#include "mesh.h"
#include "light.h"
#include "meshrenderer.h"
#include "Engine.h"

EMSCRIPTEN_BINDINGS(my_module) {

    emscripten::class_<std::vector<Light>>("LightVector")
        .constructor<>()
        .function("size", &std::vector<Light>::size);
    
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
        .function("moveBackward", &Camera::moveBackward)
        .function("moveRight", &Camera::moveRight)
        .function("moveLeft", &Camera::moveLeft)
        .function("moveUp", &Camera::moveUp)
        .function("moveDown", &Camera::moveDown)
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
        .function("getWidth", &Window::GetWidth)
        .function("getHeight", &Window::GetHeight);

    // Debug utils
    emscripten::function("printMatrix", &printMatrix);
    
    // Light class
    emscripten::class_<Light>("Light")
        .constructor<>()
        .constructor<Light::Type>()
        .function("setPosition", &Light::setPosition)
        .function("setDirection", &Light::setDirection)
        .function("setColor", &Light::setColor)
        .function("setIntensity", &Light::setIntensity)
        .function("setRange", &Light::setRange)
        .function("setType", &Light::setType)
        .function("getPosition", &Light::getPosition)
        .function("getDirection", &Light::getDirection)
        .function("getColor", &Light::getColor)
        .function("getIntensity", &Light::getIntensity)
        .function("getRange", &Light::getRange)
        .function("getType", &Light::getType);
    
    // Light type enum
    emscripten::enum_<Light::Type>("LightType")
        .value("Point", Light::Type::Point)
        .value("Directional", Light::Type::Directional)
        .value("Spot", Light::Type::Spot);
    
    // Mesh class
    emscripten::class_<Mesh>("Mesh")
        .constructor<const std::string&>()
        .function("draw", &Mesh::Draw);

    emscripten::class_<MeshInstance>("MeshInstance")
        .constructor<>();
    
    // MeshRenderer class
    emscripten::class_<MeshRenderer>("MeshRenderer")
        .constructor<>()
        .function("setMesh", &MeshRenderer::SetMesh, emscripten::allow_raw_pointers())
        .function("addInstance", &MeshRenderer::AddInstance)
        .function("clearInstances", &MeshRenderer::ClearInstances)
        .function("setTransform", &MeshRenderer::SetTransform)
        .function("setMaterial", &MeshRenderer::SetMaterial)
        .function("setLight", &MeshRenderer::SetLight)
        .function("setLights", &MeshRenderer::SetLights)
        .function("render", &MeshRenderer::Render)
        .function("loadShaders", &MeshRenderer::LoadShaders)
        .function("useShader", &MeshRenderer::UseShader);

    // OpenGL bindings
    emscripten::function("glEnable", &glEnable);
    emscripten::function("glDisable", &glDisable);
    emscripten::constant("GL_DEPTH_TEST", GL_DEPTH_TEST);

    emscripten::class_<InputDevice>("InputDevice")
        .constructor<int>()
        .function("isButtonDown", &InputDevice::IsButtonDown)
        .function("isButtonUp", &InputDevice::IsButtonUp)
        .function("isButtonPressed", &InputDevice::IsButtonPressed)
        .function("isButtonReleased", &InputDevice::IsButtonReleased)
        .function("isButtonHeld", &InputDevice::IsButtonHeld);
    
    emscripten::class_<Keyboard>("Keyboard")
        .constructor<int>()
        .function("isKeyDown", &Keyboard::IsKeyDown)
        .function("isKeyUp", &Keyboard::IsKeyUp)
        .function("isKeyPressed", &Keyboard::IsKeyPressed)
        .function("isKeyReleased", &Keyboard::IsKeyReleased)
        .function("isKeyHeld", &Keyboard::IsKeyHeld);
    
    emscripten::class_<Mouse>("Mouse")
        .constructor<int>()
        .function("isButtonDown", &Mouse::IsButtonDown)
        .function("isButtonUp", &Mouse::IsButtonUp)
        .function("isButtonPressed", &Mouse::IsButtonPressed)
        .function("isButtonReleased", &Mouse::IsButtonReleased)
        .function("isButtonHeld", &Mouse::IsButtonHeld)
        .function("getX", &Mouse::GetX)
        .function("getY", &Mouse::GetY)
        .function("getDeltaX", &Mouse::GetDeltaX)
        .function("getDeltaY", &Mouse::GetDeltaY);
    
    // Engine class
    emscripten::class_<Engine>("Engine")
        .constructor<>()
        .function("initialize", emscripten::select_overload<bool(const std::string&)>(&Engine::Initialize))
        .function("processEvents", &Engine::ProcessEvents)
        .function("handleKeyboardInput", &Engine::HandleKeyboardInput)
        .function("handleMouseMoveEvent", &Engine::HandleMouseMoveEvent)
        .function("handleMouseButtonEvent", &Engine::HandleMouseButtonEvent)
        .function("update", &Engine::Update)
        .function("render", &Engine::Render)
        .function("loadModel", &Engine::LoadModel)
        .function("handleFileDrop", &Engine::HandleFileDrop)
        .function("getKeyboard", &Engine::GetKeyboard, emscripten::allow_raw_pointers())
        .function("getMouse", &Engine::GetMouse, emscripten::allow_raw_pointers())
        .function("getCamera", &Engine::GetCamera, emscripten::allow_raw_pointers());

}











