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

        renderer.Render();

        window.SwapBuffers();
    }

    window.Destroy();

    return 0;
}
#endif