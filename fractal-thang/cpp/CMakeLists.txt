cmake_minimum_required(VERSION 3.13)
project(fractal_renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten specific settings
set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".html")

# Add source files
set(SOURCES
    Renderer.cpp
)

# Create the library
add_library(fractal_renderer STATIC ${SOURCES})

# Set Emscripten specific flags
target_compile_options(fractal_renderer PRIVATE
    -s WASM=1
    -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
    -s EXPORTED_FUNCTIONS='["_malloc", "_free"]'
    -s EXPORT_ES6=1
    -s MODULARIZE=1
    -s USE_ES6_IMPORT_META=1
    -s ENVIRONMENT=web
    -s ALLOW_MEMORY_GROWTH=1
    -s MAXIMUM_MEMORY=4GB
    -s EXPORT_NAME="createFractalRenderer"
    -s EXPORTED_CLASSES="Renderer"
) 