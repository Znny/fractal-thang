//std includes
#include <iostream>
#include <thread>
#include <chrono>

// local headers
#include "trianglerenderer.h"
#include "window.h"

// main function
#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {
    Window window;
    TriangleRenderer renderer;
    if (!window.Init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }

    while (!window.ShouldClose()) {
        window.Clear();
        window.PollEvents();

        renderer.SetModelMatrix(glm::mat4(1.0f));
        renderer.SetViewMatrix(glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        renderer.SetProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f));
        renderer.Render();

        window.SwapBuffers();
    }

    window.Destroy();

    return 0;
}
#endif