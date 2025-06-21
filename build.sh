#!/bin/bash
set -e  # Exit on error

# Default target
TARGET="web"

# Parse command line arguments
while [ "$1" != "" ]; do
    case $1 in
        --target=* )    TARGET="${1#*=}"
                        ;;
        * )             echo "Unknown option: $1"
                        echo "Usage: ./build.sh [--target=web|native]"
                        exit 1
    esac
    shift
done

# Validate target
if [ "$TARGET" != "web" ] && [ "$TARGET" != "native" ]; then
    echo "Error: Invalid target '$TARGET'. Must be 'web' or 'native'"
    exit 1
fi

# Ensure we're in the correct directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

echo "Building for target: $TARGET"

# Clean any existing build artifacts
make clean 2>/dev/null || true

if [ "$TARGET" = "web" ]; then
    # Build for web using emscripten
    echo "Building for web..."
    fractal-web/ems.sh emmake make web
    
    # Copy the output files to the fractal-web public/wasm directory  
    echo "Copying output files to fractal-web/public/wasm/..."
    cp fractal-core/build/gen.* fractal-web/public/wasm/
    
    echo "Web build complete! Output files copied to fractal-web/public/wasm/"
else
    # Build for native
    echo "Building for native..."
    make native
    
    echo "Native build complete! Executable created at fractal-core/build/fractal"
    echo "You can run it with: ./fractal-core/build/fractal"
fi


