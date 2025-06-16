#!/bin/bash
set -e  # Exit on error

# Ensure we're in the correct directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# enter build directory
cd build

# Clean any existing build artifacts
make clean 2>/dev/null || true

# Configure the build
echo "Configuring build..."
../ems.sh ./configure

# Build the project using emmake
echo "Building project..."
../ems.sh emmake make -j$(nproc)

# Copy the output files to the fractal-thang directory  
echo "Copying output files..."
cp gen.* ../../fractal-thang/src/imports/

echo "Build complete! Open gen.html in a web browser to see the result."


