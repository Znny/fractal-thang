#!/bin/bash

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to build web target
build_native() {
    echo "[HR] Building native target..."
    ../build.sh --target=native
    echo "[HR] Build complete!"
}

# Initial build
build_native

# Try different file watching methods
if command_exists inotifywait; then
    echo "[HR] Using inotifywait for file watching..."
    while inotifywait -r -e modify,create,delete src/; do
        build_native
    done
elif command_exists entr; then
    echo "[HR] Using entr for file watching..."
    find src -name '*.cpp' | entr -r ../build.sh --target=native
else
    echo "[HR] No file watcher found. Installing inotify-tools..."
    echo "[HR] Please run: sudo apt install inotify-tools"
    echo "[HR] Or install entr: sudo apt install entr"
    echo "[HR] Falling back to manual polling (press Ctrl+C to stop)..."
    
    # Fallback: simple polling
    while true; do
        sleep 2
        # Check if any .cpp files have been modified in the last 2 seconds
        if find src -name '*.cpp' -newer /tmp/last_build 2>/dev/null | grep -q .; then
            build_native
            touch /tmp/last_build
        fi
    done
fi