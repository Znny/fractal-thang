#include "Engine.h"
#include <iostream>
#include <cstring>
#include <memory>

Engine* Engine::engineInstance = nullptr;

Engine::Engine() {
    if(!engineInstance) {
        engineInstance = this;
    }

    startTime = std::chrono::high_resolution_clock::now();
    lastTime = startTime;
    
    // Initialize lights
    lights = {
        Light(Light::Type::Point, glm::vec3(0.0f, 15.0f, -45.0f/2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 100.0f, 10.0f),
        Light(Light::Type::Point, glm::vec3(-5.5f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, 10.0f),
        Light(Light::Type::Point, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f, 10.0f),
        Light(Light::Type::Point, glm::vec3(5.5f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 10.0f, 10.0f)
    };
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize(const std::string& canvasId) {
    #ifdef __EMSCRIPTEN__
    std::cout << "Engine::Initialize() called with canvasId: " << canvasId << std::endl;
    #endif
    
    this->canvasId = canvasId;
    this->isWebPlatform = true;
    
    // Create systems
    window = std::make_unique<Window>(1920, 1080);
    camera = std::make_unique<Camera>();
    
    // Initialize window with canvas ID for web
    if (!window->Init(canvasId)) {
        std::cerr << "Failed to initialize window" << std::endl;
        return false;
    }
    
    return InitializeCommon();
}

bool Engine::Initialize(int argc, char** argv) {
    this->isWebPlatform = false;
    
    // Create systems
    window = std::make_unique<Window>(1920, 1080);
    camera = std::make_unique<Camera>();
    
    // Initialize window for native (no canvas ID needed)
    if (!window->Init()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return false;
    }
    
    return InitializeCommon();
}

bool Engine::InitializeCommon() {
    std::cout << "Engine::InitializeCommon() called" << std::endl;
    meshRenderer = std::make_unique<MeshRenderer>();
    lightRenderer = std::make_unique<LightRenderer>();
    triangleRenderer = std::make_unique<TriangleRenderer>();
    
    std::cout << "Creating keyboard and mouse" << std::endl;
    keyboard = std::make_unique<Keyboard>();
    mouse = std::make_unique<Mouse>();

    #ifndef __EMSCRIPTEN__
    glfwSetKeyCallback(window->GetWindow(), HandleKeyboardInput);
    glfwSetCursorPosCallback(window->GetWindow(), HandleMouseMoveEvent);
    glfwSetMouseButtonCallback(window->GetWindow(), HandleMouseButtonEvent);
    #endif
    // Load default model
    std::string modelPath = "columns.fbx";
    LoadModel(modelPath);

    
    // Initialize OpenGL state
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    
    #ifdef __EMSCRIPTEN__
    std::cout << "OpenGL state initialized" << std::endl;
    #endif
    
    // Setup camera
    camera->setPerspective(45.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    
    #ifdef __EMSCRIPTEN__
    std::cout << "Camera setup complete" << std::endl;
    #endif
    
    // Setup renderers
    if (!meshRenderer->LoadShaders("pbr.vert", "pbr.frag")) {
        std::cerr << "Failed to load PBR shaders" << std::endl;
        return false;
    }
    
    #ifdef __EMSCRIPTEN__
    std::cout << "Shaders loaded successfully" << std::endl;
    #endif
    
    // Setup default scene
    SetupDefaultScene();
    
    #ifdef __EMSCRIPTEN__
    std::cout << "Default scene setup complete" << std::endl;
    std::cout << "Engine initialization completed successfully" << std::endl;
    #endif
    
    return true;
}

void Engine::SetupDefaultScene() {
    // Add mesh instances
    MeshInstance instance;
    instance.transform = glm::mat4(1.0f);
    instance.transform = glm::rotate(instance.transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    meshRenderer->AddInstance(instance);
    
    // Add more instances
    for (int i = 0; i < 3; i++) {
        instance.transform = glm::translate(instance.transform, glm::vec3(0.0f, 15.0f, 0.0f));
        meshRenderer->AddInstance(instance);
    }
    
    meshRenderer->SetLights(lights);
    triangleRenderer->SetModelMatrix(glm::mat4(1.0f));
}

void Engine::ProcessEvents() {
    #ifndef __EMSCRIPTEN__
    glfwPollEvents();
    #endif
    
    // Update input device states for frame transitions
    if (keyboard) {
        keyboard->Update();
    }
    if (mouse) {
        mouse->Update();
    }
}

void Engine::Update(float dt) {

    // Process events from input devices and windew
    ProcessEvents();
    
    // Update light animations
    float time = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - startTime).count();
    UpdateLightAnimation(time);
    
    // Handle camera movement with input
    if (keyboard && camera) {
        float moveSpeed = 5.0f;
        /*
        // WASD movement - using IsButtonDown for continuous movement
        if (keyboard->IsButtonDown(GLFW_KEY_W)) {
            camera->moveForward(moveSpeed * dt);
        }
        if (keyboard->IsButtonDown(GLFW_KEY_S)) {
            camera->moveForward(-moveSpeed * dt);
        }
        if (keyboard->IsButtonDown(GLFW_KEY_A)) {
            camera->moveRight(-moveSpeed * dt);
        }
        if (keyboard->IsButtonDown(GLFW_KEY_D)) {
            camera->moveRight(moveSpeed * dt);
        }
        if (keyboard->IsButtonDown(GLFW_KEY_Q)) {
            camera->moveUp(-moveSpeed * dt);
        }
        if (keyboard->IsButtonDown(GLFW_KEY_E)) {
            camera->moveUp(moveSpeed * dt);
        }
        
        // Example: Print when a key is first pressed (not held)
        if (keyboard->IsButtonPressed(GLFW_KEY_SPACE)) {
            std::cout << "Space key was just pressed!" << std::endl;
        }
            */
    }
    /*
    // Handle mouse camera rotation
    if (mouse && camera) {
        static bool firstMouse = true;
        static double lastX = 0, lastY = 0;
        
        // Manual button mapping since we don't have GLFW in web
        const int MOUSE_BUTTON_LEFT = 0;
        const int MOUSE_BUTTON_RIGHT = 1;
        
        if (mouse->IsButtonDown(MOUSE_BUTTON_LEFT)) { // Use left mouse button for rotation
            double currentX = mouse->GetX();
            double currentY = mouse->GetY();
            
            if (firstMouse) {
                lastX = currentX;
                lastY = currentY;
                firstMouse = false;
            }
            
            double deltaX = lastX - currentX;
            double deltaY = lastY - currentY;
            
            float rotateSpeed = 0.5f; // Reduced sensitivity
            camera->rotateYaw(deltaX * rotateSpeed * dt);
            camera->rotatePitch(deltaY * rotateSpeed * dt);
            
            lastX = currentX;
            lastY = currentY;
        } else {
            firstMouse = true;
        }
    }
    */
}

void Engine::Render() {
    
    window->Clear();
    
    // Render mesh
    meshRenderer->SetLights(lights);
    meshRenderer->Render(camera->getViewMatrix(), camera->getProjectionMatrix(), camera->getPosition());
    
    // Render light spheres
    lightRenderer->Render(lights, camera->getViewMatrix(), camera->getProjectionMatrix());
    
    // Render triangle
    triangleRenderer->SetViewMatrix(camera->getViewMatrix());
    triangleRenderer->SetProjectionMatrix(camera->getProjectionMatrix());
    triangleRenderer->Render();
    
    window->SwapBuffers();
}

void Engine::Shutdown() {
    // Cleanup will be handled by unique_ptr destructors
}







void Engine::UpdateLightAnimation(float time) {
    float t = std::sin(time);

    // Light 0: oscillate in y
    lights[0].setPosition(glm::vec3(0.0f, 7.5f, -45.0f/2.0f) + glm::vec3(0.0f, 7.5f * t, 0.0f));

    // Lights 1-3: oscillate in z, offset by phase
    for (int i = 1; i <= 3; ++i) {
        float phase = (float)i * glm::pi<float>() * 2.0f / 3.0f; // 120 deg offset per light
        float zOsc = -25.5f + 25.5f * std::sin(time + phase); // oscillate between 0 and -90
        float x = (i == 1) ? -5.5f : (i == 3) ? 5.5f : 0.0f;
        float y = 5.0f;
        lights[i].setPosition(glm::vec3(x, y, zOsc));
    }
}

void Engine::LoadModel(const std::string& path) {
    mesh = std::make_unique<Mesh>(path);
    if (!mesh || mesh->vertices.empty()) {
        std::cerr << "Failed to load " << path << std::endl;
        return;
    }
    meshRenderer->SetMesh(mesh.get());
}

void Engine::HandleFileDrop(const std::string& filePath) {
    // Check if it's a supported model format
    std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
    if (extension == "fbx" || extension == "obj" || extension == "gltf") {
        LoadModel(filePath);
    }
}

#ifndef __EMSCRIPTEN__
void Engine::HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(engineInstance) {
        engineInstance->GetKeyboard()->HandleButtonEvent(key, action == GLFW_PRESS || action == GLFW_REPEAT);
    }
}

void Engine::HandleMouseMoveEvent(GLFWwindow* window, double xpos, double ypos) {
    if(engineInstance) {
        engineInstance->GetMouse()->HandleMouseMoveEvent(xpos, ypos);
    }
}   

void Engine::HandleMouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
    if(engineInstance) {
        engineInstance->GetMouse()->HandleButtonEvent(button, action == GLFW_PRESS);
    }
}
#else
void Engine::HandleKeyboardInput(int key, bool bIsDown) {
    if(engineInstance) {
        engineInstance->GetKeyboard()->HandleButtonEvent(key, bIsDown);
    }
}

void Engine::HandleMouseMoveEvent(double xPos, double yPos) {
    if(engineInstance) {
        engineInstance->GetMouse()->HandleMouseMoveEvent(xPos, yPos);
    }
}

void Engine::HandleMouseButtonEvent(int button, bool bIsDown) {
    if(engineInstance) {
        engineInstance->GetMouse()->HandleButtonEvent(button, bIsDown);
    }
}
#endif