#ifndef RENDERER_H
#define RENDERER_H

// C++ standard library
#include <string>

// gl header
#include "glreq.h"

class Renderer
{
    public:
        Renderer(int width = 800, int height = 600);
        ~Renderer();

        bool InitGLContext(const std::string& canvasId = "");
        void DestroyGLContext();
        void Resize(int newWidth, int newHeight);
        void Render();
        void Clear();

        int GetWidth() { return width; }
        int GetHeight() { return height; }

    private:
        int width;
        int height;
        
        #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;
        #else
        GLFWwindow* window;
        #endif
        
        bool glInitialized;
};

#endif