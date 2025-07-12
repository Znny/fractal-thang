#ifndef GLREQ_H
#define GLREQ_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten/html5_webgl.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif

