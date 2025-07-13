//std includes
#include <iostream>
#include <thread>

#include "glreq.h"

// local headers
#include "trianglerenderer.h"
#include "window.h"
#include "camera.h"

// Forward declarations
void Tick(float dt, Window& window, Camera& camera, float moveSpeed, float rotateSpeed, 
          bool& rightMouseDown, double& lastMouseX, double& lastMouseY, bool& firstMouse);

// main function
#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {
    Window window;
    TriangleRenderer renderer;
    Camera camera;
    
    if (!window.Init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }

    // Set up camera with window dimensions
    camera.setPerspective(45.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);
    camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

    // Camera control variables
    float moveSpeed = 5.0f; // Units per second
    float rotateSpeed = 2.0f; // Radians per second
    bool rightMouseDown = false;
    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstMouse = true;

    // Timing variables - calculate delta time each frame
    float dt = 0.0f;
    auto thisTime = std::chrono::high_resolution_clock::now();
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose()) {
        thisTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(thisTime - lastTime).count();
        lastTime = thisTime;

        window.Clear();
        window.PollEvents();

        // Handle camera controls
        Tick(dt, window, camera, moveSpeed, rotateSpeed, rightMouseDown, lastMouseX, lastMouseY, firstMouse);

        //update the camera
        renderer.SetModelMatrix(glm::mat4(1.0f));
        renderer.SetViewMatrix(camera.getViewMatrix());
        renderer.SetProjectionMatrix(camera.getProjectionMatrix());

        renderer.Render();

        window.SwapBuffers();
    }

    window.Destroy();

    return 0;
}

// Tick function for frame-rate independent camera controls
void Tick(float dt, Window& window, Camera& camera, float moveSpeed, float rotateSpeed, 
          bool& rightMouseDown, double& lastMouseX, double& lastMouseY, bool& firstMouse) {
    
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
#endif