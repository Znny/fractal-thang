// renderer.cpp

// C++ standard library
#include <iostream>
#include <string>
#include <memory>

// local headers
#include "renderer.h"
#include "ShaderProgram.h"

namespace {
ShaderProgram* triangleShader = nullptr;
unsigned int VAO = 0, VBO = 0;
bool triangleSetup = false;
}

Renderer::Renderer(int width, int height)
{
    glInitialized = false;

    this->width = width;
    this->height = height;
    
    #ifdef __EMSCRIPTEN__
    glContext = 0;
    #else
    window = nullptr;
    #endif
}

Renderer::~Renderer()
{
    DestroyGLContext();
}

bool Renderer::InitGLContext(const std::string& canvasId)
{
    if (glInitialized) {
        std::cout << "GL context already initialized" << std::endl;
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
    #ifndef __EMSCRIPTEN__
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    #endif
    #endif
    
    // Set up viewport
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glInitialized = true;
    std::cout << "OpenGL context initialized successfully" << std::endl;
    return true;
}

void Renderer::DestroyGLContext()
{
    if (!glInitialized) {
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
    
    glInitialized = false;
}

void Renderer::Resize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    
    if (glInitialized) {
        glViewport(0, 0, width, height);
    }
}

void Renderer::Render()
{
    if (!glInitialized) {
        return;
    }

    // Clear the screen
    Clear();

    // --- Triangle rendering ---
    if (!triangleSetup) {
        triangleShader = new ShaderProgram();

        #ifdef __EMSCRIPTEN__
        triangleShader->AttachShaderFromFile("/shaders/passthrough.vert", GL_VERTEX_SHADER);
        triangleShader->AttachShaderFromFile("/shaders/passthrough.frag", GL_FRAGMENT_SHADER);
        triangleShader->Link();
        #else
        triangleShader->AttachShaderFromFile("fractal-core/shaders/passthrough.vert", GL_VERTEX_SHADER);
        triangleShader->AttachShaderFromFile("fractal-core/shaders/passthrough.frag", GL_FRAGMENT_SHADER);
        triangleShader->Link();
        #endif

        float vertices[] = {
            // positions      // texcoords
            0.0f,  0.5f, 0.0f,  0.5f, 1.0f,
           -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texcoord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        triangleSetup = true;
    }

    triangleShader->Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    #ifndef __EMSCRIPTEN__
    // Swap buffers for native platform
    if (window) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    #endif
}

void Renderer::Clear()
{
    if (!glInitialized) {
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

