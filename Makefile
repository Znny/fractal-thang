# Build configuration with web and native targets
CC = gcc           # Default C compiler
CXX = g++          # Default C++ compiler

# Directory structure
BUILD_DIR = fractal-core/build
NATIVE_OBJ_DIR = $(BUILD_DIR)/obj-native
WEB_OBJ_DIR = $(BUILD_DIR)/obj-web
SRC_DIR = fractal-core/src
INCLUDE_DIR = fractal-core/include
EMSCRIPTEN_INCLUDE_DIR = ${EMSDK}/upstream/emscripten/system/include

# Create object directories
$(shell mkdir -p $(NATIVE_OBJ_DIR))
$(shell mkdir -p $(WEB_OBJ_DIR))

# Source files
ALL_SRC = $(wildcard $(SRC_DIR)/*.cpp)
# Filter out emscripten-specific files for native build
NATIVE_SRC = $(filter-out $(SRC_DIR)/embinding.cpp,$(ALL_SRC))
WEB_SRC = $(ALL_SRC)

NATIVE_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(NATIVE_OBJ_DIR)/%.o,$(NATIVE_SRC))
WEB_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(WEB_OBJ_DIR)/%.o,$(WEB_SRC))

# Native build configuration
NATIVE_CC = gcc
NATIVE_CXX = g++
NATIVE_CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR)
NATIVE_CXXFLAGS = $(NATIVE_CFLAGS) -std=c++17
NATIVE_TARGET = $(BUILD_DIR)/fractal

# Web build configuration
WEB_CC = emcc
WEB_CXX = em++
WEB_CFLAGS = -s WASM=1 \
             -s MODULARIZE=1 \
             -s NO_EXIT_RUNTIME=1 \
             -s ENVIRONMENT=web \
             -s ALLOW_MEMORY_GROWTH=1 \
             -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] \
             -s EXPORT_NAME="Fractal"

WEB_INCLUDE_FLAGS = -I$(INCLUDE_DIR) \
             -I$(EMSCRIPTEN_INCLUDE_DIR)

WEB_LDFLAGS = -lembind -lstdc++
WEB_CXXFLAGS = $(WEB_CFLAGS)
WEB_TSDFLAGS = --emit-tsd main.d.ts
WEB_TARGET = $(BUILD_DIR)/main.js

# Default target
all: native

# Native target
native: $(NATIVE_TARGET)

$(NATIVE_TARGET): $(NATIVE_OBJ)
	@echo "Linking native executable..."
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) -o $(NATIVE_TARGET) $^

# Web target
web: $(WEB_TARGET)

$(WEB_TARGET): $(WEB_OBJ)
	@echo "Linking WebAssembly module..."
	$(WEB_CXX) $(WEB_CXXFLAGS) -o $(WEB_TARGET) $^ $(WEB_TSDFLAGS) $(WEB_INCLUDE_FLAGS) $(WEB_LDFLAGS)

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
	rm -f $(NATIVE_OBJ) $(WEB_OBJ) $(NATIVE_TARGET) $(WEB_TARGET) $(BUILD_DIR)/*.wasm
	rm -rf $(NATIVE_OBJ_DIR) $(WEB_OBJ_DIR)

.PHONY: all native web clean
