#pragma once

// STL includes
#include <memory>
#include <vector>
#include <chrono>
#include <string>

// Engine includes
#include "window.h"
#include "camera.h"

//Rendering components
#include "mesh.h"
#include "light.h"

// Renderers
#include "meshrenderer.h"
#include "lightrenderer.h"
#include "trianglerenderer.h"

// Input
#include "input/Keyboard.h"
#include "input/Mouse.h"

class Engine {
    static Engine* engineInstance;
public:
    static Engine* GetInstance() { return engineInstance; }

    Engine();
    ~Engine();
    
    // Core lifecycle - web version
    bool Initialize(const std::string& canvasId = "");
    // Core lifecycle - native version
    bool Initialize(int argc, char** argv);
 
    void ProcessEvents();
    void Update(float dt);
    void Render();
    void Shutdown();
    
    // Getters for platform-specific code
    Window* GetWindow() const { return window.get(); }
    Camera* GetCamera() const { return camera.get(); }
    Keyboard* GetKeyboard() const { return keyboard.get(); }
    Mouse* GetMouse() const { return mouse.get(); }

    #ifdef __EMSCRIPTEN__
    void HandleKeyboardInput(int key, bool bIsDown);
    void HandleMouseMoveEvent(double xPos, double yPos);
    void HandleMouseButtonEvent(int button, bool bIsDown);
    #else
    static void HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void HandleMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
    static void HandleMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
    #endif
    
    // File handling (future)
    void LoadModel(const std::string& path);
    void HandleFileDrop(const std::string& filePath);

private:
    // Core systems
    std::unique_ptr<Window> window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<MeshRenderer> meshRenderer;
    std::unique_ptr<LightRenderer> lightRenderer;
    std::unique_ptr<TriangleRenderer> triangleRenderer;
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Keyboard> keyboard;
    std::unique_ptr<Mouse> mouse;

    
    // State
    std::vector<Light> lights;
    
    // Timing
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lastTime;
    
    // Platform
    std::string canvasId;
    bool isWebPlatform = false;
    
    void UpdateLightAnimation(float time);
    
    // Initialization helpers
    bool InitializeCommon();
    void SetupDefaultScene();
};