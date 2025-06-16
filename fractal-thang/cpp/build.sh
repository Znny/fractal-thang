#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Configure with Emscripten
emcmake cmake ..

# Build
emmake make

# Copy the output files to the public directory
cp fractal_renderer.* ../public/ 