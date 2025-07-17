import './App.css'
import { useState, useEffect, useRef } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule, TriangleRenderer, Window, Mat4, Vec3, Camera, MeshRenderer, Mesh, Light, MeshInstance } from './cpp/main.d'

export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  const [window, setWindow] = useState<Window>()
  const [renderer, setRenderer] = useState<TriangleRenderer>()
  const [camera, setCamera] = useState<Camera>()
  const [meshRenderer, setMeshRenderer] = useState<MeshRenderer>()
  const [mesh, setMesh] = useState<Mesh>()
  const [light, setLight] = useState<Light>()
  const canvasRef = useRef<HTMLCanvasElement>(null)
  
  // Camera control state
  const [keys, setKeys] = useState<Set<string>>(new Set())
  const [mouseDown, setMouseDown] = useState(false)
  const [lastMousePos, setLastMousePos] = useState({ x: 0, y: 0 })
  const [firstMouse, setFirstMouse] = useState(true)
  const animationRef = useRef<number | undefined>(undefined)
  const keysRef = useRef<Set<string>>(new Set())
  const meshRendererRef = useRef<MeshRenderer | undefined>(undefined)

  // Key code constants for layout-independent input
  const KEY_CODES = {
    W: 'KeyW',
    A: 'KeyA', 
    S: 'KeyS',
    D: 'KeyD',
    Q: 'KeyQ',
    E: 'KeyE'
  } as const

  useEffect(() => {
    console.log('Loading WebAssembly module')
    Fractal().then((instance) => {
      setWasmModule(instance)
      
      // Create renderer instance
      const rendererInstance = new instance.TriangleRenderer()
      setRenderer(rendererInstance)

      // Create window instance
      const windowInstance = new instance.Window()
      setWindow(windowInstance)

      // Create camera instance
      const cameraInstance = new instance.Camera()
      setCamera(cameraInstance)

      // Create a simple light
      const lightInstance = new instance.Light()
      lightInstance.setPosition(new instance.Vec3(2.0, 2.0, 2.0))
      lightInstance.setColor(new instance.Vec3(1.0, 1.0, 1.0))
      lightInstance.setIntensity(1.0)
      setLight(lightInstance)
    }
    ).catch((err) => {
      console.error('Failed to load WebAssembly module:', err)
    })
    console.log('WebAssembly module loaded')
  }, [])

  useEffect(() => {
    if (window && renderer && wasmModule && camera && light && canvasRef.current) {
      console.log('Initializing OpenGL context...')
      const success = window.init('#'+canvasRef.current.id)
      if (success) {
        console.log('OpenGL context initialized successfully')

        wasmModule.glEnable(wasmModule.GL_DEPTH_TEST)
        // Set canvas size
        const canvas = canvasRef.current
        canvas.width = window.getWidth()
        canvas.height = window.getHeight()

        camera.setPerspective(45, window.getWidth() / window.getHeight(), 0.1, 100)
        camera.setPosition(new wasmModule.Vec3(0, 0, 5))
        camera.setRotation(new wasmModule.Vec3(0, 0, 0))
        camera.setScale(new wasmModule.Vec3(1, 1, 1))

        // Create mesh renderer instance
        const meshRendererInstance = new wasmModule.MeshRenderer()
        setMeshRenderer(meshRendererInstance)
        meshRendererRef.current = meshRendererInstance

        // Initialize mesh renderer with the instance directly
        initializeMeshRenderer(meshRendererInstance)

        // Start the render loop
        startRenderLoop()
      } else {
        console.error('Failed to initialize OpenGL context')
      }
    }
  }, [renderer, wasmModule, camera, light])

  // Input handlers
  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      setKeys(prev => {
        const newKeys = new Set(prev).add(e.code)
        keysRef.current = newKeys
        return newKeys
      })
    }

    const handleKeyUp = (e: KeyboardEvent) => {
      setKeys(prev => {
        const newKeys = new Set(prev)
        newKeys.delete(e.code)
        keysRef.current = newKeys
        return newKeys
      })
    }

    const handleMouseDown = (e: MouseEvent) => {
      if (e.button === 2) { // Right click
        setMouseDown(true)
        setFirstMouse(true)
      }
    }

    const handleMouseUp = (e: MouseEvent) => {
      if (e.button === 2) {
        setMouseDown(false)
      }
    }

    const handleMouseMove = (e: MouseEvent) => {
      if (mouseDown) {
        if (firstMouse) {
          setLastMousePos({ x: e.clientX, y: e.clientY })
          setFirstMouse(false)
        } else {
          const deltaX = lastMousePos.x - e.clientX // Inverted for natural feel
          const deltaY = lastMousePos.y - e.clientY // Inverted Y for natural feel
          setLastMousePos({ x: e.clientX, y: e.clientY })
          
          // Apply rotation (will be handled in tick function)
          if (camera) {
            const rotateSpeed = 0.01
            camera.rotateYaw(deltaX * rotateSpeed)
            camera.rotatePitch(deltaY * rotateSpeed)
          }
        }
      }
    }

    const handleContextMenu = (e: MouseEvent) => {
      e.preventDefault() // Prevent right-click context menu
    }

    // Add event listeners to browser window
    globalThis.addEventListener('keydown', handleKeyDown)
    globalThis.addEventListener('keyup', handleKeyUp)
    globalThis.addEventListener('mousedown', handleMouseDown)
    globalThis.addEventListener('mouseup', handleMouseUp)
    globalThis.addEventListener('mousemove', handleMouseMove)
    globalThis.addEventListener('contextmenu', handleContextMenu)

    // Cleanup
    return () => {
      globalThis.removeEventListener('keydown', handleKeyDown)
      globalThis.removeEventListener('keyup', handleKeyUp)
      globalThis.removeEventListener('mousedown', handleMouseDown)
      globalThis.removeEventListener('mouseup', handleMouseUp)
      globalThis.removeEventListener('mousemove', handleMouseMove)
      globalThis.removeEventListener('contextmenu', handleContextMenu)
    }
  }, [mouseDown, firstMouse, lastMousePos, camera])

  const initializeMeshRenderer = (rendererInstance?: MeshRenderer) => {
    const mrenderer = rendererInstance || meshRenderer
    if (!mrenderer || !wasmModule) {
      console.error("Mesh renderer or wasmModule is not initialized")
      return
    }

    try {
      // Load shaders (you'll need to provide the shader files)
      const shaderLoaded = mrenderer.loadShaders("pbr.vert", "pbr.frag")
      if (!shaderLoaded) {
        console.warn("Failed to load mesh shaders, using default")
      }

      const meshInstance = new wasmModule.Mesh('columns.fbx')
      mrenderer.setMesh(meshInstance)

      // Create a simple mesh instance
      const instance = new wasmModule.MeshInstance()
      mrenderer.addInstance(instance)

      // Set the light
      mrenderer.setLight(0, light!)

      console.log("Mesh renderer initialized successfully")
    } catch (error) {
      console.error("Failed to initialize mesh renderer:", error)
    }
  }

  const tick = (dt: number) => {
    if (!camera) return

    const moveSpeed = 5.0 // Units per second
    const distance = moveSpeed * dt

    // Handle keyboard input for movement (layout-independent)
    if (keysRef.current.has(KEY_CODES.W)) {
      camera.moveForward(distance)
    }
    if (keysRef.current.has(KEY_CODES.S)) {
      camera.moveForward(-distance)
    }
    if (keysRef.current.has(KEY_CODES.A)) {
      camera.moveRight(-distance)
    }
    if (keysRef.current.has(KEY_CODES.D)) {
      camera.moveRight(distance)
    }
    if (keysRef.current.has(KEY_CODES.Q)) {
      camera.moveUp(-distance)
    }
    if (keysRef.current.has(KEY_CODES.E)) {
      camera.moveUp(distance)
    }
  }

  const startRenderLoop = () => {
    let lastTime = performance.now()

    const renderLoop = (currentTime: number) => {
      const dt = (currentTime - lastTime) / 1000.0 // Convert to seconds
      lastTime = currentTime

      // Update camera controls
      tick(dt)

      // Render mesh if available (primary renderer)
      if (meshRendererRef.current && camera) {
        if(Math.floor(currentTime / 16.67) % 60 === 0) {
          console.log("Rendering mesh")
        }
        const viewMatrix = camera.getViewMatrix()
        const projectionMatrix = camera.getProjectionMatrix()
        const viewPos = camera.getPosition()
        meshRendererRef.current!.render(viewMatrix, projectionMatrix, viewPos)
      }
      // Fallback to triangle renderer if mesh renderer is not available
      else if (renderer && camera) {
        if(Math.floor(currentTime / 16.67) % 60 === 0) {
          console.log("Rendering triangle")
        }
        const identityMatrix = new wasmModule!.Mat4(1)
        renderer.setModelMatrix(identityMatrix)
        renderer.setViewMatrix(camera.getViewMatrix())
        renderer.setProjectionMatrix(camera.getProjectionMatrix())
        renderer.render()
      }
      
      // Debug: Log camera position every 60 frames (once per second at 60fps)
      if (camera && Math.floor(currentTime / 16.67) % 60 === 0) { // 16.67ms = 60fps
        const pos = camera.getPosition()
        //console.log('Camera position:', pos)
      }

      // Continue loop
      animationRef.current = requestAnimationFrame(renderLoop)
    }

    animationRef.current = requestAnimationFrame(renderLoop)
  }

  // Cleanup animation frame on unmount
  useEffect(() => {
    return () => {
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current)
      }
    }
  }, [])

  return (
    <div className="app">
      <h1>Fractal Web</h1>
      <p>WebAssembly module {wasmModule ? 'loaded' : 'loading...'}</p>
      <p>Controls: WASD to move, QE for up/down, Right-click + drag to rotate</p>

      <canvas
        ref={canvasRef}
        id="fractal-canvas"
        style={{
          border: '1px solid #ccc',
          width: '800px',
          height: '600px'
        }}
      />
    </div>
  )
}