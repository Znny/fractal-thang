//std includes
#include <iostream>
#include "renderer.h"
#include <thread>
#include <chrono>

#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {

    Renderer renderer;
    if (!renderer.InitGLContext()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
    renderer.DestroyGLContext();

    return 0;
}
#endif