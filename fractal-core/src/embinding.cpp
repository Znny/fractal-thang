#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "trianglerenderer.h"
#include "window.h"

EMSCRIPTEN_BINDINGS(my_module) {
    
    //Classes
    // Renderer class
    emscripten::class_<TriangleRenderer>("TriangleRenderer")
        .constructor<>()
        .function("render", &TriangleRenderer::Render);

    emscripten::class_<Window>("Window")
        .constructor<>()
        .function("init", &Window::Init)
        .function("destroy", &Window::Destroy)
        .function("resize", &Window::Resize)
        .function("swapBuffers", &Window::SwapBuffers)
        .function("pollEvents", &Window::PollEvents)
        .function("getWidth", &Window::GetWidth)
        .function("getHeight", &Window::GetHeight);
}










