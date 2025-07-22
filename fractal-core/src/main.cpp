//std includes
#include <iostream>
#include <thread>
#include <memory>

#include "glreq.h"

// local headers
#include "Engine.h"

#ifndef __EMSCRIPTEN__
// Global variables
Engine* engine = nullptr;

// Forward declarations
bool init(int argc, char** argv);
void resizeCallback(GLFWwindow* window, int width, int height);

// main function
int main(int argc, char** argv) {
    
    // Initialize the application
    if (!init(argc, argv)) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    // Timing variables - calculate delta time each frame
    float dt = 0.0f;
    auto lastTime = std::chrono::high_resolution_clock::now();
    float CameraSpeed = 5.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;

    while (!engine->GetWindow()->ShouldClose()) 
    {
        auto thisTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(thisTime - lastTime).count();
        lastTime = thisTime;
        #ifndef __EMSCRIPTEN__
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_ESCAPE)) {
            engine->GetWindow()->Close();
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_W)) {
            engine->GetCamera()->moveForward(CameraSpeed * dt);
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_S)) {
            engine->GetCamera()->moveBackward(CameraSpeed * dt);
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_A)) {
            engine->GetCamera()->moveLeft(CameraSpeed * dt);
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_D)) {
            engine->GetCamera()->moveRight(CameraSpeed * dt);
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_Q)) {
            engine->GetCamera()->moveUp(CameraSpeed * dt);
        }
        if(engine->GetKeyboard()->IsButtonDown(GLFW_KEY_E)) {
            engine->GetCamera()->moveDown(CameraSpeed * dt);
        }

        if(engine->GetMouse()->IsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) 
        {
            lastX = engine->GetMouse()->GetX();
            lastY = engine->GetMouse()->GetY();
        }
        if(engine->GetMouse()->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) 
        {
            float xpos = engine->GetMouse()->GetX();
            float ypos = engine->GetMouse()->GetY();
            float xoffset = lastX - xpos;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            engine->GetCamera()->rotateYaw(xoffset * 1.0f * dt);
            engine->GetCamera()->rotatePitch(yoffset * 1.0f * dt);
        }
        #endif
        engine->Update(dt);
        engine->Render();
    }
    
    delete engine;
    return 0;
}


// Initialization function
bool init(int argc, char** argv) {
    engine = new Engine();
    
    if (!engine->Initialize(argc, argv)) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return false;
    }

    // Set up resize callback
    glfwSetWindowSizeCallback(engine->GetWindow()->GetWindow(), resizeCallback);

    return true;
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
        if (engine && engine->GetCamera()) {
            engine->GetCamera()->setPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
        }
    }
}
#endif