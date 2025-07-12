# Modular 3D Rendering Engine Roadmap

A structured plan of milestones to guide development and ensure incremental progress.

###############################################################
## Phase 1: Project Setup & Basics
###############################################################

Establish the foundational repository structure, build pipelines, and verify end-to-end rendering capability.

### Milestones
**TODO**
*DONE*

- *Setup GitHub repository*
  - Initialize repository, add a README, and configure .gitignore

- *Configure frontend & toolchain*
  - Add React, Vite, and npm
  - Integrate Emscripten toolchain for WASM builds

- *Create basic C++ test code*
  - Scaffold minimal C++ project and add build scripts
  - Ensure Emscripten compilation works without errors

- *Verify WASM loading*
  - Implement JS glue code to load and instantiate the WASM module in the browser

- *Enable Vite hot-reloading*
  - Configure Vite dev server for frontend hot-reload

- *Support C++ code hot-reloading*
  - Integrate workflow so changes in C++ trigger automatic rebuild and reload in the browser

- *Validate fresh-build pipelines*
  - Verify project builds successfully from a clean clone for both native and web targets

- **Integrate cross-platform testing framework**
  - Add a testing setup that can run tests in native and WASM environments

- **Implement basic rendering example**
  - Render a triangle or fullscreen quad using simple vertex and fragment shaders
  - Ensure the example works and appears correctly in both native (OpenGL ES 3.0) and browser (WebGL2) builds

###############################################################
## Phase 2: Core Rendering Infrastructure
###############################################################

Define and implement the foundational rendering systems, shader management, and pipeline architecture.

### Milestones

- **Window & Context Management**
  - Abstract window creation for native (SDL2/GLFW) and web (HTML5 canvas via Emscripten)
  - Initialize OpenGL ES 3.0 context or WebGL2 context with desired attributes (depth, stencil, antialiasing)
  - Handle context loss and resize events uniformly across platforms

- **Shader Management**
  - Create ShaderProgram class for loading, compiling, and linking GLSL/GLSL ES sources
  - Define standardized vertex input layouts and attribute binding utilities
  - Implement Uniform Buffer Object (UBO) support: layout definitions, buffer creation, binding points
  - Provide detailed shader error reporting (file/line mapping) in native logs and browser console
  - Integrate hot-reload: file-watcher for shader directories, runtime recompile, and automatic pipeline update

- **Mesh & Quad Primitives**
  - Implement reusable mesh loader for simple shapes (quad, cube, sphere)
  - Build VAO/VBO utilities for vertex formats and instancing support
  - Validate basic uploads and draw calls with test meshes

- **Pipeline Manager Setup**
  - Extend RenderPipelineManager to register render passes and framebuffers
  - Configure G-buffer attachments for color, normal, depth, and material data
  - Define clear, bind, and blit routines for multi-pass sequencing
  - Research deferred rendering concepts:
    - Study G-buffer construction, lighting passes, and final composition stages
    - Compare forward, deferred, and hybrid rendering workflows to identify trade-offs for different object types (SDF, PBR, volumetric)
  - Auto-Batching & Instancing:
    - Implement automatic detection and grouping of instanced draw calls for repeated objects
    - Learn when to use instancing versus individual draw calls or indirect draw strategies to optimize CPU/GPU overhead
    - Create instanced buffer management utilities and integrate them into the pipeline manager for bulk uploads
  - Render Mode Management:
    - Ensure the pipeline can selectively include or exclude mutually exclusive modes (e.g., volumetric splats vs mesh deferred)
    - Provide hooks for future rendering techniques (Gaussian splats, NERFs) to register their own passes

- **SceneGraph & RenderObject Integration**
  - Finalize RenderObject API: registration, parameter updates, lifecycle
  - Implement scene graph flattening to compute world-space transforms each frame
  - Batch flattened objects by RenderMode and dispatch to appropriate pipeline

- **Validation & Testing**
  - Write unit tests for context creation, shader compile/link, and pipeline setup
  - Add visual smoke tests: render clear color, wireframe quad, and basic geometry to verify end-to-end

###############################################################
## Phase 3: Camera & Post-Processing
###############################################################

Set up input handling, camera controls, and initial post-processing effects.

### Milestones

- **Input Management Setup**
  - Integrate InputManager module for mouse, keyboard, and gamepad via SDL2/GLFW or browser events
  - Ensure consistent event polling and callback interfaces across native and web

- **Input Mapping & Binding**
  - Design a flexible binding system to map raw inputs to actions (e.g., move, look, toggle UI)
  - Support runtime rebindable controls and presets

- **6DOF Camera Implementation**
  - Develop CameraController in C++: position/orientation updates, smoothing, acceleration
  - Expose camera parameters to React UI for debug and adjustments

- **Post-Processing Pipeline**
  - Extend FBO chain to support multi-pass post effects
  - Implement configurable render pass ordering and enable/disable flags

- **Basic Effects**
  - Glow/Bloom: threshold, blur, and composite stages
  - Depth of Field: circle-of-confusion calculation and blur pass

###############################################################
## Phase 4: Fractal Engine MVP
###############################################################

Bring specialized fractal rendering logic online using the core pipeline.

### Milestones

- **Integrate FractalDEFunctions Shaders**
  - Import GLSL modules for Mandelbulb and Julia fractals
  - Configure shader include mechanism for hot-swapping additional formulas

- **Implement FractalRenderer Module**
  - Extend RenderPipelineManager with a DE-specific pass
  - Bind fractal uniforms (exponent, iterations, offsets) via UBO or separate uniform blocks
  - Dispatch full-screen quad draw calls for distance estimation rendering

- **Expose Fractal Parameters via WASM Bridge**
  - Define JS-accessible functions to update fractal parameters at runtime
  - Ensure parameter changes trigger shader recompilation or uniform updates

- **Connect ParameterPanel Controls**
  - Map UI sliders and inputs to fractal parameters (iteration depth, color palettes, zoom level)
  - Implement real-time feedback loop: UI changes immediately reflect in rendering

- **Validate Dynamic Rendering**
  - Create visual tests for smooth parameter interpolation (morphing behavior)
  - Benchmark frame rate and iteration performance

- **Support Fractal Morphing & Keyframing**
  - Implement simple interpolation manager for timed fractal transitions
  - Add keyframe controls in UI or scriptable API

###############################################################
## Phase 5: Extensibility & Asset Management
###############################################################

Enable plugin-based extensions and robust asset handling across platforms.

### Milestones

- **RenderableTypeRegistry Enhancements**
  - Define plugin interface for new renderable types (shader-based, compute-based)
  - Implement dynamic registration and initialization API in RenderableTypeRegistry
  - Create example plugin demonstrating integration of a custom render mode

- **AssetManager Core**
  - Build asynchronous loader supporting URLs (web) and file paths (native)
  - Cache loaded assets (shaders, models, textures, fractal parameters) to avoid redundant loads
  - Expose load progress and error reporting to the UI and logs

- **Hot-Reload & Watchers**
  - Extend file-watcher to track asset directories (shaders, JSON scenes, models)
  - Trigger reloading of changed assets at runtime without restarting the application

- **SceneLoader Integration**
  - Parse .scene/.json files to instantiate RenderObjects and set initial parameters
  - Validate scene data against schema and report errors

- **Debug & Diagnostic UI**
  - Add asset browser panel to list loaded assets and manual reload options
  - Display plugin registry entries and allow enabling/disabling plugins

###############################################################
## Phase 6: Multiplayer Integration
###############################################################

Implement real-time client-server synchronization for shared exploration modes.

### Milestones

- **WebSocket Infrastructure**
  - Set up a minimal Node.js WebSocket server for client connections
  - Create client-side NetworkingManager to connect and maintain socket

- **Protocol Design**
  - Define message schemas for:
    - Handshake and room join
    - Player transform updates
    - Parameter & state synchronization
    - Game events (e.g., sardines found)

- **State Synchronization**
  - Implement efficient transform interpolation for remote players
  - Sync fractal parameter changes and keyframe events across clients

- **Multiplayer Modes**
  - Build Sardines hide-and-seek logic: designate hider, broadcast location on find
  - Implement collaborative exploration mode: shared free-fly camera updates

- **Security & Reliability**
  - Validate incoming messages to prevent out-of-range values
  - Handle reconnection and state resync on network errors

###############################################################
## Phase 7: Physics & Interaction
###############################################################

Add interactive physics simulations for gameplay mechanics.

### Milestones

- **Rapier Integration**
  - Compile Rapier physics library into WASM and native targets
  - Expose physics world API for creating rigid bodies and colliders

- **Physics API & Bindings**
  - Provide JS/WASM bindings for physics functions (apply force, step simulation)
  - Define C++ wrappers to abstract engine-specific types

- **Projectile & Trace Demo**
  - Implement sphere-spawn on user input, apply initial velocity
  - Simulate collisions and bounce using Rapier callbacks
  - Draw visual trace lines or particle effects along projectile path

- **Synchronization & Determinism**
  - Ensure physics simulation is deterministic or reconcile discrepancies
  - Optionally use server-authoritative simulation for multiplayer

- **Performance Tuning**
  - Profile physics step times and adjust sub-stepping or collision granularity
  - Enable multi-threaded physics stepping via Emscripten pthreads or native threads

###############################################################
## Phase 8: Advanced Rendering Techniques
###############################################################

Extend capabilities with Gaussian splatting, NERF, and volumetrics.

### Milestones

- **Gaussian Splatting**
  - Implement point-cloud renderer using instanced quads and Gaussian kernels
  - Add volume blending stage in the deferred compositor

- **Neural Radiance Fields (NERF)**
  - Load pre-trained NERF datasets (e.g., from JSON or binary files)
  - Ray-march volumetric data with optimized sampling and ray termination

- **Volumetric Lighting & Effects**
  - Prototype fog, light shafts, and participating media in screen-space
  - Expose density and falloff controls in UI

- **Compute Shader Paths (Future)**
  - Evaluate Emscripten/WebGL2 support for compute shaders or use fragment-based compute
  - Design fallback strategies for browsers lacking compute support

#########################################################################
###############################################################
## Phase 9: Polishing & Packaging
###############################################################
#########################################################################

Prepare the engine for release, documentation, and community consumption.

### Milestones

- **Documentation & Tutorials**
  - Write comprehensive guide for engine setup and API usage
  - Create example projects demonstrating each render mode and plugin

- **Performance Optimization**
  - Profile CPU/GPU hotspots, reduce draw calls, optimize shader complexity
  - Implement LOD controls and adaptive quality presets

- **CI/CD Pipeline**
  - Configure GitHub Actions to build and test native and web targets on push
  - Automate deployment of web demo to GitHub Pages or similar

- **Release Packaging**
  - Build native installers for supported platforms (Windows, macOS, Linux)
  - Bundle web-friendly assets, minify WASM glue, and publish demo site

- **Community & Support**
  - Set up issue templates, contribution guidelines, and a discussion forum
  - Plan versioning strategy and roadmap milestones for future releases

---

*This roadmap is maintained separately for clarity; see the main spec for architectural details.*