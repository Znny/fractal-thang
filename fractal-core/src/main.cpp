//std includes
#include <iostream>
#include <thread>
#include <chrono>

// local headers
#include "renderer.h"

// main function
#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {

    Renderer renderer;
    if (!renderer.InitGLContext()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }

    int frameCount = 0;
    while (frameCount < 5000) {
        renderer.Render();
        frameCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(10));
    renderer.DestroyGLContext();

    return 0;
}
#endif