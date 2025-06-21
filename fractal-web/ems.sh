#!/bin/bash

# Path to the Emscripten SDK directory (update if needed)
EMSDK_DIR="$HOME/emsdk"

echo EMSDK_DIR: $EMSDK_DIR
#echo "Files in EMSDK_DIR:"
#ls -la "$EMSDK_DIR"

# Source the Emscripten environment
source "$EMSDK_DIR/emsdk_env.sh" 


# Try running emcc to verify the environment
echo "Testing emcc..."
emcc --version


# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <emcc|emar|em++|etc> [arguments...]"
    exit 1
fi

# Run the specified tool with the remaining arguments
"$@"
