#!/bin/bash
set -e  # Exit on error

# Default target
TARGET="web"
CLEAN=false

# Parse command line arguments
while [ "$1" != "" ]; do
    case $1 in
        --target=* )    TARGET="${1#*=}"
                        ;;
        --clean )       CLEAN=true
                        ;;
        * )             echo "Unknown option: $1"
                        echo "Usage: ./build.sh [--target=web|native] [--clean]"
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

# Ensure TypeScript is available
if ! command -v tsc &> /dev/null; then
    echo "TypeScript not found in PATH, using local installation..."
    export PATH="$PWD/fractal-web/node_modules/.bin:$PATH"
fi

# Only clean if explicitly requested
if [ "$CLEAN" = true ]; then
    make clean 2>/dev/null || true
fi

if [ "$TARGET" = "web" ]; then

    #ensure zlib is built with emscripten
    if [ ! -f external/zlib/libz.a ]; then
        cd external/zlib
        emconfigure ./configure --disable-shared
        emmake make libz.a
        cd ../..
    else
        echo "Zlib lib already exists"
    fi

    #check if assimp lib exists
    if [ ! -f external/assimp/lib/libassimp.a ]; then
    #ensure assimp is built with emscripten
        cd external/assimp
        emcmake cmake CMakeLists.txt -DZLIB_LIBRARY=../zlib/libz.a -DZLIB_INCLUDE_DIR=../zlib
        emcmake cmake . -DCMAKE_BUILD_TYPE=Release -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DASSIMP_BUILD_SAMPLES=OFF -DBUILD_SHARED_LIBS=OFF
        emmake make -j$(nproc)
        cd ../..
    else
        echo "Assimp lib already exists"
    fi


    # Build for web using emscripten
    echo "Building for web..."
    fractal-web/ems.sh emmake make web
    
    # Copy the output files to the fractal-web public/wasm directory  
    echo "Copying output files to fractal-web/public/wasm/..."
    mkdir -p fractal-web/public/
    cp fractal-core/build/main.wasm fractal-web/public/
    cp fractal-core/build/main.data fractal-web/public/


    # Copy the output files to the fractal-web src/cpp directory
    mkdir -p fractal-web/src/cpp
    
    # Check if the last two lines already contain "export default Fractal;" and add it if not
    if ! tail -n 2 fractal-core/build/main.js | grep -q "export default Fractal;"; then
        echo "export default Fractal;" >> fractal-core/build/main.js
    fi
    
    cp fractal-core/build/main.js fractal-web/src/cpp/main.js
    cp fractal-core/build/main.d.ts fractal-web/src/cpp/main.d.ts
    
    echo "Web build complete! Output files copied to fractal-web/public/wasm/"
else
    # Build for native
    echo "Building for native..."
    make native
    
    echo "Native build complete! Executable created at fractal-core/build/fractal"
    echo "You can run it with: ./fractal-core/build/fractal"
fi


