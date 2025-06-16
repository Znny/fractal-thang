# Fractal Visualization Project

A project for rendering and visualizing fractals using C++ (compiled to WebAssembly) and React.

## Project Structure

- `fractal-ems/`: C++ and Emscripten build system
  - Handles compilation of C++ code to WebAssembly
  - Contains build scripts and configuration
- `fractal-thang/`: React frontend
  - Web-based visualization using WebGL
  - Built with Vite and TypeScript

## Prerequisites

- Node.js and pnpm
- Emscripten SDK
- CMake
- C++ compiler

## Setup

1. Install dependencies:
```bash
cd fractal-thang
pnpm install
```

2. Build C++ code:
```bash
cd fractal-ems
./build.sh
```

3. Start development server:
```bash
cd fractal-thang
pnpm dev
```

## Development

The project uses a hybrid approach:
- C++ code is compiled to WebAssembly using Emscripten
- React frontend handles the UI and WebGL rendering
- Communication between C++ and JavaScript is handled through Emscripten bindings

## License

[Add your chosen license here] 