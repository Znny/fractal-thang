#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten/html5_webgl.h>
#else
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#endif

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        bool InitGLContext(const std::string& canvasId = "");
        void DestroyGLContext();
        void Resize(int newWidth, int newHeight);
        void Render();
        void Clear();

        int GetWidth() { return width; }
        int GetHeight() { return height; }
        int* GetBuffer() { return buffer; }

    private:
        int width;
        int height;
        int* buffer;
        
        #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;
        #else
        GLFWwindow* window;
        #endif
        
        bool glInitialized;
};

#endif