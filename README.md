# Fractal Visualization Project

A project for rendering and visualizing fractals using C++ (compiled to WebAssembly) and React.

## Project Structure

- `fractal-core/`: C++ core library and build system
  - Handles compilation of C++ code to WebAssembly and native targets
  - Contains build scripts, hot reloading, and configuration
  - Includes temporary test code (marked as garbage) for build system validation
- `fractal-web/`: React frontend
  - Web-based visualization using WebGL
  - Built with Vite and TypeScript
- `tests/`: Test suite
  - Google Test integration for C++ code
  - Separate build system for test compilation and execution

## Prerequisites

- Node.js and pnpm
- Emscripten SDK (for WebAssembly compilation)
- CMake (for Google Test)
- C++ compiler (GCC/Clang)
- File watching tools (optional): `inotify-tools` or `entr`

## Setup


1. Install dependencies:
```bash
#install emsdk(if not already installed)
cd ../
git clone https://github.com/emscripten-core/emsdk
cd emsdk
./emsdk install latest
./emsdk activate latest
cd fractal-thang

#install web dependencies
cd fractal-web
pnpm install

#init and pull dependencies
git submodule init
git submodule update


```

2. Setup Google Test (one-time setup):
```bash
cd tests
make setup-gtest
```

3. Initial build (optional - will happen automatically with dev-full):
```bash
./build.sh --target=web
```

## Development Workflow

### Full Development Mode (Recommended)
This runs both the Vite dev server and C++ hot reloading simultaneously:

```bash
cd fractal-web
pnpm run dev-full
```

This command:
- Starts the Vite development server for the React frontend
- Runs the C++ hot reloading script (`hr.sh`) that watches for changes in `fractal-core/src/`
- Automatically rebuilds WebAssembly when C++ code changes
- Provides instant feedback for both frontend and backend development

### Manual Development Mode
If you prefer to control hot reloading manually:

1. Start C++ hot reloading (optional):
```bash
cd fractal-core
./hr.sh
```

2. Start the web development server:
```bash
cd fractal-web
pnpm dev
```

### Testing

Run the test suite:
```bash
cd tests
make run
```

Run tests with coverage (requires lcov):
```bash
cd tests
make coverage
```

## Build Targets

- **Web**: `./build.sh --target=web` - Compiles C++ to WebAssembly
- **Native**: `./build.sh --target=native` - Compiles C++ to native executable
- **Clean**: `./build.sh --clean` - Cleans build artifacts

## Development Notes

- The current `gen.h` and `gen.cpp` files are temporary placeholder code for testing the build system
- Hot reloading requires file watching tools (`inotify-tools` or `entr`) for optimal performance
- The build system supports both native and WebAssembly targets from the same C++ source
- Tests are compiled separately to avoid conflicts with Emscripten-specific code

## License

[Add your chosen license here] 