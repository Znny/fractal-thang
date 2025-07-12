#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../include/renderer.h"

EMSCRIPTEN_BINDINGS(my_module) {
    
    //Classes
    // Renderer class
    emscripten::class_<Renderer>("Renderer")
        .constructor<>()
        .function("initGLContext", &Renderer::InitGLContext)
        .function("destroyGLContext", &Renderer::DestroyGLContext)
        .function("resize", &Renderer::Resize)
        .function("render", &Renderer::Render)
        .function("clear", &Renderer::Clear)
        .function("getWidth", &Renderer::GetWidth)
        .function("getHeight", &Renderer::GetHeight);
}










