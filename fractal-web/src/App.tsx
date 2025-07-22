import './App.css'
import { useState, useEffect, useRef } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule, Engine, Keyboard, Mouse } from './cpp/main.d'

export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  const [engine, setEngine] = useState<Engine>()

  const canvasRef = useRef<HTMLCanvasElement>(null)
  const animationRef = useRef<number | undefined>(undefined)


  useEffect(() => {
    console.log('Loading WebAssembly module')
    Fractal().then((instance: any) => {
      setWasmModule(instance)
      
      // Create engine instance
      const engineInstance = new instance.Engine()
      setEngine(engineInstance)
    }
    ).catch((err: any) => {
      console.error('Failed to load WebAssembly module:', err)
    })
    console.log('WebAssembly module loaded')
  }, [])

  useEffect(() => {
    if (engine && wasmModule && canvasRef.current) {
      console.log('Initializing Engine...')
      
      // Initialize engine with canvas ID
      const canvasId = canvasRef.current.id
      const success = engine.initialize('#'+canvasId)
      if (success) {
        console.log('Engine initialized successfully')

        // Set canvas size to match the engine's expected size
        const canvas = canvasRef.current
        canvas.width = 1920
        canvas.height = 1080

        // Start the render loop
        startRenderLoop()
      } else {
        console.error('Failed to initialize engine')
      }
    }
  }, [engine, wasmModule])

  // Set up keyboard event listeners
  useEffect(() => {
    if (!engine) return

    const keyToScancode: { [key: string]: number } = {
      'KeyW': 87,
      'KeyA': 65,
      'KeyS': 83,
      'KeyD': 68,
      'KeyQ': 81,
      'KeyE': 69,
      'ArrowUp': 38,
      'ArrowDown': 40,
      'ArrowLeft': 37,
      'ArrowRight': 39,
      'Space': 32,
      'ShiftLeft': 16,
      'ShiftRight': 16,
      'ControlLeft': 17,
      'ControlRight': 17,
    }

    const handleMouseMove = (event: MouseEvent) => {
      const deltaX = event.clientX
      const deltaY = event.clientY
      engine.handleMouseMoveEvent(deltaX, deltaY)
    }

    const handleMouseDown = (event: MouseEvent) => {
      const button = event.button
      engine.handleMouseButtonEvent(button, true)
    }

    const handleMouseUp = (event: MouseEvent) => {
      const button = event.button
      engine.handleMouseButtonEvent(button, false)
    }

    const handleKeyDown = (event: KeyboardEvent) => {
      //console.log(event.code)
      //console.log(event.key)
      const scancode = keyToScancode[event.code] ?? event.key.charCodeAt(0)
      engine.handleKeyboardInput(scancode, true)
    }

    const handleKeyUp = (event: KeyboardEvent) => {
      //console.log(event.code)
      //console.log(event.key)
      const scancode = keyToScancode[event.code] ?? event.key.charCodeAt(0)
      engine.handleKeyboardInput(scancode, false)
    }

    document.addEventListener('keydown', handleKeyDown)
    document.addEventListener('keyup', handleKeyUp)
    document.addEventListener('mousemove', handleMouseMove)
    document.addEventListener('mousedown', handleMouseDown)
    document.addEventListener('mouseup', handleMouseUp)
    return () => {
      document.removeEventListener('keydown', handleKeyDown)
      document.removeEventListener('keyup', handleKeyUp)
      document.removeEventListener('mousemove', handleMouseMove)
      document.removeEventListener('mousedown', handleMouseDown)
    }
  }, [engine])

  // Helper function to get keyboard state
  const getKeyboardState = () => {
    if (!engine) return null
    return engine.getKeyboard()
  }

  const getMouseState = () => {
    if (!engine) return null
    return engine.getMouse()
  }

  const startRenderLoop = () => {
    console.log('Starting render loop...')
    let lastTime = performance.now()
    
    const renderLoop = () => {
      const currentTime = performance.now()
      const deltaTime = (currentTime - lastTime) / 1000.0 // Convert to seconds
      lastTime = currentTime

      if (engine) {
        // TODO: Fix keyboard input checking after rebuilding TypeScript definitions
        // Get fresh keyboard reference from engine each frame
        const currentKeyboard = getKeyboardState()
        const currentMouse = getMouseState()
        const currentCamera = engine.getCamera()
        const cameraMoveSpeed = 5.0 // Units per second

        //handle keyboard input
        if(!currentKeyboard) {
          console.log('Keyboard not found')
        }
        else if(!currentCamera) {
          console.log('Camera not found')
        }
        else {
          // Test keyboard input
          if(currentKeyboard.isKeyDown(32)) {
          }
          if(currentKeyboard.isKeyDown('A'.charCodeAt(0))) {
            currentCamera.moveLeft(cameraMoveSpeed * deltaTime)
          }
          if(currentKeyboard.isKeyDown('D'.charCodeAt(0))) {
            currentCamera.moveRight(cameraMoveSpeed * deltaTime)
          }
          if(currentKeyboard.isKeyDown('S'.charCodeAt(0))) {
            currentCamera.moveBackward(cameraMoveSpeed * deltaTime)
          }
          if(currentKeyboard.isKeyDown('W'.charCodeAt(0))) {
            currentCamera.moveForward(cameraMoveSpeed * deltaTime)
          }
          if(currentMouse && currentCamera && currentMouse.isButtonDown(0)) {
            const deltaX = -currentMouse.getDeltaX()
            const deltaY = -currentMouse.getDeltaY()
            currentCamera.rotateYaw(deltaX * 0.1 * deltaTime)
            currentCamera.rotatePitch(deltaY * 0.1 * deltaTime)
          }
        }

        // Mouse input is handled via events, not polling
        // The engine's mouse input system handles the camera rotation automatically
        engine.update(deltaTime) // Use actual delta time
        engine.render()
      }
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
          width: '1920px',
          height: '1080px',
          position: 'absolute',
          top: 0,
          left: 0,
          zIndex: 1000
        }}
      />
    </div>
  )
}