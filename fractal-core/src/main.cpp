//std includes
#include <iostream>
#include <thread>
#include <memory>

#include "glreq.h"

// local headers
#include "trianglerenderer.h"
#include "window.h"
#include "camera.h"
#include "meshrenderer.h"
#include "mesh.h"
#include "lightrenderer.h"

#ifndef __EMSCRIPTEN__
// Global variables
Window* window = nullptr;
TriangleRenderer* trianglerenderer = nullptr;
Camera* camera = nullptr;
MeshRenderer* meshRenderer = nullptr;
LightRenderer* lightRenderer = nullptr;

//light positions
std::vector<Light> lights = {
    Light(Light::Type::Point, glm::vec3(0.0f, 15.0f, -45.0f/2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 100.0f, 10.0f),
    Light(Light::Type::Point, glm::vec3(-5.5f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, 10.0f),
    Light(Light::Type::Point, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f, 10.0f),
    Light(Light::Type::Point, glm::vec3(5.5f, 4.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 10.0f, 10.0f)
};

//time variables
const auto startTime = std::chrono::high_resolution_clock::now();
auto thisTime = std::chrono::high_resolution_clock::now();
auto lastTime = std::chrono::high_resolution_clock::now();

// Forward declarations
bool init(int argc, char** argv);
void Tick(float dt, Window& window, Camera& camera, float moveSpeed, float rotateSpeed, 
          bool& rightMouseDown, double& lastMouseX, double& lastMouseY, bool& firstMouse);
void resizeCallback(GLFWwindow* window, int width, int height);

// main function
int main(int argc, char** argv) {
    
    // Initialize the application
    if (!init(argc, argv)) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    // Camera control variables
    float moveSpeed = 5.0f; // Units per second
    float rotateSpeed = 2.0f; // Radians per second
    bool rightMouseDown = false;
    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstMouse = true;

    // Timing variables - calculate delta time each frame
    float dt = 0.0f;

    while (!window->ShouldClose()) {
        thisTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(thisTime - lastTime).count();
        lastTime = thisTime;

        window->Clear();
        window->PollEvents();

        // Handle camera controls
        Tick(dt, *window, *camera, moveSpeed, rotateSpeed, rightMouseDown, lastMouseX, lastMouseY, firstMouse);

        // Render mesh
        meshRenderer->SetLights(lights);
        meshRenderer->Render(camera->getViewMatrix(), camera->getProjectionMatrix(), camera->getPosition());

        // Render light spheres
        lightRenderer->Render(lights, camera->getViewMatrix(), camera->getProjectionMatrix());

        // Render triangle
        trianglerenderer->SetViewMatrix(camera->getViewMatrix());
        trianglerenderer->SetProjectionMatrix(camera->getProjectionMatrix());
        trianglerenderer->Render();

        window->SwapBuffers();
    }

    window->Destroy();

    return 0;
}

// Initialization function
bool init(int argc, char** argv) {
    window = new Window(1920,1080);
    trianglerenderer = new TriangleRenderer();
    camera = new Camera();

    if (!window->Init()) { 
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    // Set up resize callback
    glfwSetWindowSizeCallback(window->GetWindow(), resizeCallback);

    //light and mesh renderer must be initialized after window is initialized and we have a valid OpenGL context
    meshRenderer = new MeshRenderer();
    lightRenderer = new LightRenderer();

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);

    // Set up camera with window dimensions
    camera->setPerspective(45.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

    std::string modelPath = "columns.fbx";
    if(argc > 1) {
        if(strcmp(argv[1], "--load") == 0) {
            // If a custom model is specified, resolve it as an asset path
            modelPath = argv[2];
        }
    }

    // Load test mesh
    auto mesh = std::make_shared<Mesh>(modelPath);
    if (!mesh || mesh->vertices.empty()) {
        std::cerr << "Failed to load " << modelPath << std::endl;
        return false;
    }
    meshRenderer->SetMesh(mesh.get());
    
    // Load PBR shaders
    if (!meshRenderer->LoadShaders("pbr.vert", "pbr.frag")) {
        std::cerr << "Failed to load PBR shaders" << std::endl;
        return false;
    }
    
    // Add a mesh instance
    MeshInstance instance;
    instance.transform = glm::mat4(1.0f);
    instance.transform = glm::rotate(instance.transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    meshRenderer->AddInstance(instance);
    instance.transform = glm::translate(instance.transform, glm::vec3(0.0f, 15.0f, 0.0f));
    meshRenderer->AddInstance(instance);
    instance.transform = glm::translate(instance.transform, glm::vec3(0.0f, 15.0f, 0.0f));
    meshRenderer->AddInstance(instance);
    instance.transform = glm::translate(instance.transform, glm::vec3(0.0f, 15.0f, 0.0f));
    meshRenderer->AddInstance(instance);

    // Set up lighting using initializer list
    meshRenderer->SetLights(lights);

    trianglerenderer->SetModelMatrix(glm::mat4(1.0f));

    return true;
}

// Tick function for frame-rate independent camera controls
void Tick(float dt, Window& window, Camera& camera, float moveSpeed, float rotateSpeed, 
          bool& rightMouseDown, double& lastMouseX, double& lastMouseY, bool& firstMouse) {

    // Oscillate white light 0 in y, and lights 1-3 in z, with phase offsets
    float time = std::chrono::duration<float>(thisTime - startTime).count();
    float t = std::sin(time);

    // Light 0: oscillate in y
    lights[0].setPosition(glm::vec3(0.0f, 7.5f, -45.0f/2.0f) + glm::vec3(0.0f, 7.5f * t, 0.0f));

    // Lights 1-3: oscillate in z, offset by 30 in phase
    for (int i = 1; i <= 3; ++i) {
        float phase = (float)i * glm::pi<float>() * 2.0f / 3.0f; // 120 deg offset per light
        float zOsc = -25.5f + 25.5f * std::sin(time + phase); // oscillate between 0 and -90
        float x = (i == 1) ? -5.5f : (i == 3) ? 5.5f : 0.0f;
        float y = 5.0f;
        lights[i].setPosition(glm::vec3(x, y, zOsc));
    }

    
    // Handle keyboard input for movement
    if (window.IsKeyPressed(GLFW_KEY_W)) {
        camera.moveForward(moveSpeed * dt);
    }
    if (window.IsKeyPressed(GLFW_KEY_S)) {
        camera.moveForward(-moveSpeed * dt);
    }
    if (window.IsKeyPressed(GLFW_KEY_A)) {
        camera.moveRight(-moveSpeed * dt);
    }
    if (window.IsKeyPressed(GLFW_KEY_D)) {
        camera.moveRight(moveSpeed * dt);
    }
    if (window.IsKeyPressed(GLFW_KEY_Q)) {
        camera.moveUp(-moveSpeed * dt);
    }
    if (window.IsKeyPressed(GLFW_KEY_E)) {
        camera.moveUp(moveSpeed * dt);
    }


    // Handle mouse input for rotation
    if (window.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!rightMouseDown) {
            rightMouseDown = true;
            firstMouse = true;
        }
        
        double mouseX, mouseY;
        window.GetCursorPos(&mouseX, &mouseY);
        
        if (firstMouse) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }
        
        double deltaX = lastMouseX - mouseX; // Inverted for natural feel
        double deltaY = lastMouseY - mouseY; // Inverted for natural feel
        
        camera.rotateYaw(deltaX * rotateSpeed * dt);
        camera.rotatePitch(deltaY * rotateSpeed * dt);
        
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    } else {
        rightMouseDown = false;
    }
}

// Resize callback function
void resizeCallback(GLFWwindow* window, int width, int height) {
    if (width > 0 && height > 0) {
        if(window == nullptr) {
            std::cerr << "Window is nullptr in resizeCallback" << std::endl;
            return;
        }
        // Update viewport
        glViewport(0, 0, width, height);
        
        // Update camera aspect ratio
        if (camera) {
            camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
        }
    }
}
#endif