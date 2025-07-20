# Build configuration with web and native targets
CC = gcc           # Default C compiler
CXX = g++          # Default C++ compiler

# Directory structure
BUILD_DIR = fractal-core/build
NATIVE_OBJ_DIR = $(BUILD_DIR)/obj-native
WEB_OBJ_DIR = $(BUILD_DIR)/obj-web
SRC_DIR = fractal-core/src
INCLUDE_DIR = fractal-core/include
EMSCRIPTEN_INCLUDE_DIR = ${EMSDK}/upstream/emscripten/cache/sysroot/include
ASSIMP_INCLUDE_DIR = external/assimp/include
NATIVE_BIN_DIR = fractal-core/bin

# Create object directories
$(shell mkdir -p $(NATIVE_OBJ_DIR))
$(shell mkdir -p $(WEB_OBJ_DIR))

# Source files
ALL_SRC = $(wildcard $(SRC_DIR)/*.cpp)
# Filter out emscripten-specific files for native build
NATIVE_SRC = $(filter-out $(SRC_DIR)/embinding.cpp,$(ALL_SRC))
WEB_SRC = $(ALL_SRC)

# Object files
NATIVE_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(NATIVE_OBJ_DIR)/%.o,$(NATIVE_SRC))
WEB_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(WEB_OBJ_DIR)/%.o,$(WEB_SRC))

# Dependency files
NATIVE_DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(NATIVE_OBJ_DIR)/%.d,$(NATIVE_SRC))
WEB_DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(WEB_OBJ_DIR)/%.d,$(WEB_SRC))

# Include dependency files
-include $(NATIVE_DEPS)
-include $(WEB_DEPS)

# Native build configuration
NATIVE_CC = gcc
NATIVE_CXX = g++
NATIVE_CFLAGS = -Wall -Wextra -O2 -g -I$(INCLUDE_DIR) -Iexternal/glm -MMD -MP
NATIVE_LINKER_FLAGS = -lglfw -lGL -lGLEW -lassimp
NATIVE_CXXFLAGS = $(NATIVE_CFLAGS) -std=c++17
NATIVE_TARGET = $(NATIVE_BIN_DIR)/fractal

# Web build configuration
WEB_CC = emcc
WEB_CXX = em++

# Compilation flags (for creating object files)
WEB_CFLAGS = -I$(INCLUDE_DIR) \
             -Iexternal/glm \
             -I$(EMSCRIPTEN_INCLUDE_DIR) \
             -I$(ASSIMP_INCLUDE_DIR) \
             -Wno-c++20-extensions \
             -MMD -MP

# Linker flags (for final linking)
WEB_LINKER_FLAGS = -s WASM=1 \
                   -s MODULARIZE=1 \
                   -s NO_EXIT_RUNTIME=1 \
                   -s ENVIRONMENT=web \
                   -s ALLOW_MEMORY_GROWTH=1 \
                   -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] \
                   -s EXPORT_NAME="Fractal" \
                   -sMAX_WEBGL_VERSION=2 \
                   --no-entry \
                   -lembind -lstdc++ \
                   fractal-core/lib/libassimp.a \
                   external/zlib/libz.a \
                   --preload-file fractal-core/assets@/assets

WEB_CXXFLAGS = $(WEB_CFLAGS)
WEB_TSDFLAGS = --emit-tsd main.d.ts
WEB_TARGET = $(BUILD_DIR)/main.js

# Default target
all: native

# Native target
native: $(NATIVE_TARGET)

$(NATIVE_TARGET): $(NATIVE_OBJ)
	@echo "Linking native executable..."
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) -o $(NATIVE_TARGET) $^ $(NATIVE_LINKER_FLAGS)

# Web target
web: $(WEB_TARGET)

$(WEB_TARGET): $(WEB_OBJ)
	@echo "Linking WebAssembly module..."
	$(WEB_CXX) $(WEB_LINKER_FLAGS) -o $(WEB_TARGET) $^ $(WEB_TSDFLAGS)

# Compile source files to object files for native target
$(NATIVE_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< to $@ for native..."
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) -c $< -o $@

# Compile source files to object files for web target
$(WEB_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< to $@ for web..."
	$(WEB_CXX) $(WEB_CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(NATIVE_OBJ) $(WEB_OBJ) $(NATIVE_DEPS) $(WEB_DEPS) $(NATIVE_TARGET) $(WEB_TARGET) $(BUILD_DIR)/*.wasm
	rm -rf $(NATIVE_OBJ_DIR) $(WEB_OBJ_DIR)
	rm -f fractal-web/src/cpp/*
	rm -f fractal-web/public/main.*

# Preserve dependency files
.PRECIOUS: $(NATIVE_DEPS) $(WEB_DEPS)

.PHONY: all native web clean
