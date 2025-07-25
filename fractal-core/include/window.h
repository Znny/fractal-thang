#ifndef WINDOW_H
#define WINDOW_H

// C++ standard library
#include <string>

// gl header
#include "glreq.h"

class Window
{
    public:
        Window(int width = 800, int height = 600);
        ~Window();

        bool Init(const std::string& canvasId = "");
        void Clear();

        void Resize(int newWidth, int newHeight);
        void SwapBuffers();
        
        bool ShouldClose() const;
        void* GetNativeHandle() const;

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        bool IsInitialized() const { return bIsInitialized; }
        void Close() { 
            #ifndef __EMSCRIPTEN__
            glfwSetWindowShouldClose(window, GLFW_TRUE); 
            #endif
        }

        #ifndef __EMSCRIPTEN__
        GLFWwindow* GetWindow() const { return window; }
        #endif
        
        void Destroy();

    private:
        int width;
        int height;
        
        #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE glContext;
        #else
        GLFWwindow* window;
        #endif
        
        bool bIsInitialized = false;
        
    };

#endif 