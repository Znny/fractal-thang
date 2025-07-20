#!/bin/bash
set -e  # Exit on error

# Default target
TARGET="web"
CLEAN=false
EMSSH="$PWD/fractal-web/ems.sh"

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

echo "Checking dependencies..."
#ensure assimp is built for native
if [ ! -f fractal-core/bin/libassimp.so ]; then

    #check if assimp shared lib exists, build if not
    if [ ! -f external/assimp/bin/libassimp.so ]; then
        echo "Assimp shared lib not found, building..."

        cd external/assimp
        rm CMakeCache.txt
        cmake CMakeLists.txt -DASSIMP_BUILD_ZLIB=ON
        cmake --build .
        cd ../..
    fi

    echo "...copying assimp shared lib to fractal-core/bin"

    #copy assimp shared lib to fractal-core/bin
    mkdir -p fractal-core/bin
    cp external/assimp/bin/libassimp.so* fractal-core/bin/
else
    echo "Assimp shared lib exists"
fi


#ensure assimp is built for web
if [ ! -f fractal-core/lib/libassimp.a ]; then
    echo "Emscripten-built Assimp lib not found"
    #check if assimp lib exists
    if [ ! -f external/assimp/lib/libassimp.a ]; then
        echo "Building Assimp lib..."
        #check if zlib lib exists
        if [ ! -f external/zlib/libz.a ]; then
            echo "Emscripten-built Zlib lib not found, building..."
            cd external/zlib
            $EMSSH emconfigure ./configure --disable-shared
            $EMSSH emmake make libz.a
            cd ../..
        else
            echo "Emscripten-built Zlib lib exists"
        fi

        if [ ! -f external/assimp/lib/libassimp.a ]; then
        cd external/assimp
        rm CMakeCache.txt
        $EMSSH emcmake cmake CMakeLists.txt -DZLIB_LIBRARY=../zlib/libz.a -DZLIB_INCLUDE_DIR=../zlib
        $EMSSH emcmake cmake . -DCMAKE_BUILD_TYPE=Release -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DASSIMP_BUILD_SAMPLES=OFF -DBUILD_SHARED_LIBS=OFF
        $EMSSH emmake make -j$(nproc)
        cd ../..
        fi
    fi
    echo "Copying Assimp lib to fractal-core/lib"
    mkdir -p fractal-core/lib
    cp external/assimp/lib/libassimp.a fractal-core/lib/
else
    echo "Emscripten-built Assimp lib exists"
fi

echo "Building for target: $TARGET"


# Only clean if explicitly requested
if [ "$CLEAN" = true ]; then
    make clean 2>/dev/null || true
fi

if [ "$TARGET" = "web" ]; then
    # Ensure TypeScript is available
    if ! command -v tsc &> /dev/null; then
        echo "TypeScript not found in PATH, using local installation..."
        export PATH="$PWD/fractal-web/node_modules/.bin:$PATH"
    fi

    # Build for web using emscripten
    echo "Building for web..."
    $EMSSH emmake make web
    
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
    echo "Native build complete! Executable created at fractal-core/bin/fractal"
    rsync -av --ignore-existing fractal-core/assets/ fractal-core/bin/assets/
    echo "You can run it with: ./fractal-core/bin/fractal"
fi


