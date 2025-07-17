// window.cpp

// C++ standard library
#include <iostream>

// local headers
#include "window.h"

Window::Window(int width, int height)
{
    this->width = width;
    this->height = height;
    bIsInitialized = false;
    
    #ifdef __EMSCRIPTEN__
    glContext = 0;
    #else
    window = nullptr;
    #endif
}

Window::~Window()
{
    Destroy();
}

bool Window::Init(const std::string& canvasId)
{
    if (bIsInitialized) {
        std::cout << "Window " << canvasId << " already initialized" << std::endl;
        return true;
    }
    
    #ifdef __EMSCRIPTEN__
    // Initialize WebGL context for Emscripten
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.alpha = 0;
    attrs.depth = 1;
    attrs.stencil = 0;
    attrs.antialias = 1;
    attrs.premultipliedAlpha = 0;
    attrs.preserveDrawingBuffer = 0;
    attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT;
    attrs.failIfMajorPerformanceCaveat = 0;
    attrs.enableExtensionsByDefault = 1;
    attrs.explicitSwapControl = 0;
    attrs.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW;
    attrs.renderViaOffscreenBackBuffer = 0;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    
    glContext = emscripten_webgl_create_context(canvasId.c_str(), &attrs);
    if (glContext == 0) {
        std::cerr << "Failed to create WebGL context" << std::endl;
        return false;
    }
    
    EMSCRIPTEN_RESULT result = emscripten_webgl_make_context_current(glContext);
    if (result != EMSCRIPTEN_RESULT_SUCCESS) {
        std::cerr << "Failed to make WebGL context current" << std::endl;
        return false;
    }
    
    #else
    // Initialize GLFW for native platform
    std::cout << "Initializing GLFW" << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "Creating GLFW window" << std::endl;
    window = glfwCreateWindow(width, height, "Fractal Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize OpenGL extensions (GLEW would be used here in a full implementation)
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    #endif
    
    // Set up viewport
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    bIsInitialized = true;
    std::cout << "Window and OpenGL context initialized successfully" << std::endl;
    return true;
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Destroy()
{
    if (!bIsInitialized) {
        return;
    }
    
    #ifdef __EMSCRIPTEN__
    if (glContext > 0) {
        emscripten_webgl_destroy_context(glContext);
        glContext = 0;
    }
    #else
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
    }
    #endif
    
    bIsInitialized = false;
}

void Window::Resize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    
    if (bIsInitialized) {
        glViewport(0, 0, width, height);
    }
}

void Window::SwapBuffers()
{
    #ifndef __EMSCRIPTEN__
    if (window) {
        glfwSwapBuffers(window);
    }
    #endif
}

void Window::PollEvents()
{
    #ifndef __EMSCRIPTEN__
    if (window) {
        glfwPollEvents();
    }
    #endif
}

bool Window::ShouldClose() const
{
    #ifdef __EMSCRIPTEN__
    return false; // Emscripten doesn't have a close concept
    #else
    return window ? glfwWindowShouldClose(window) : true;
    #endif
}

void* Window::GetNativeHandle() const
{
    #ifdef __EMSCRIPTEN__
    return nullptr; // No native handle needed for Emscripten
    #else
    return static_cast<void*>(window);
    #endif
}

bool Window::IsKeyPressed(int key) const
{
    #ifdef __EMSCRIPTEN__
    return false; // TODO: Implement for Emscripten
    #else
    return window ? glfwGetKey(window, key) == GLFW_PRESS : false;
    #endif
}

bool Window::IsMouseButtonPressed(int button) const
{
    #ifdef __EMSCRIPTEN__
    return false; // TODO: Implement for Emscripten
    #else
    return window ? glfwGetMouseButton(window, button) == GLFW_PRESS : false;
    #endif
}

void Window::GetCursorPos(double* xpos, double* ypos) const
{
    #ifdef __EMSCRIPTEN__
    *xpos = 0.0;
    *ypos = 0.0; // TODO: Implement for Emscripten
    #else
    if (window) {
        glfwGetCursorPos(window, xpos, ypos);
    } else {
        *xpos = 0.0;
        *ypos = 0.0;
    }
    #endif
} 