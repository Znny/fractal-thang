import './App.css'
import { useState, useEffect, useRef } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule, TriangleRenderer, Window, Mat4, Vec3, Camera } from './cpp/main.d'

export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  const [window, setWindow] = useState<Window>()
  const [renderer, setRenderer] = useState<TriangleRenderer>()
  const [camera, setCamera] = useState<Camera>()
  const canvasRef = useRef<HTMLCanvasElement>(null)

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
    }
    ).catch((err) => {
      console.error('Failed to load WebAssembly module:', err)
    })
    console.log('WebAssembly module loaded')
  }, [])

  useEffect(() => {
    if (window && renderer && wasmModule && camera && canvasRef.current) {
      console.log('Initializing OpenGL context...')
      const success = window.init('#'+canvasRef.current.id)
      if (success) {
        console.log('OpenGL context initialized successfully')
        // Set canvas size
        const canvas = canvasRef.current
        canvas.width = window.getWidth()
        canvas.height = window.getHeight()


        const identityMatrix = new wasmModule.Mat4(1)
        const vec3Up = new wasmModule.Vec3(0, 1, 0)
        const vec3Eye = new wasmModule.Vec3(0, 0, 5)
        const vec3Center = new wasmModule.Vec3(0, 0, 0)
        const modelMatrix = identityMatrix;
        const viewMatrix = wasmModule.lookAt(vec3Eye, vec3Center, vec3Up)
        const projMatrix = wasmModule.perspective(45, window.getWidth() / window.getHeight(), 0.1, 100)

        camera.setPerspective(45, window.getWidth() / window.getHeight(), 0.1, 100)
        camera.setPosition(new wasmModule.Vec3(0, 0, 5))
        camera.setRotation(new wasmModule.Vec3(0, 0, 0))
        camera.setScale(new wasmModule.Vec3(1, 1, 1))

        wasmModule.printMatrix(camera.getTransformMatrix())
        wasmModule.printMatrix(viewMatrix)

        renderer.setModelMatrix(modelMatrix)
        renderer.setViewMatrix(camera.getViewMatrix())
        renderer.setProjectionMatrix(camera.getProjectionMatrix())

        //renderer.setModelMatrix(modelMatrix)
        //renderer.setViewMatrix(viewMatrix)
        //renderer.setProjectionMatrix(projMatrix)

        // Initial render
        handleRender()
      } else {
        console.error('Failed to initialize OpenGL context')
      }
    }
  }, [renderer, wasmModule])

  const handleRender = () => {
    if (renderer) {
      renderer.render()
    }
  }

  return (
    <div className="app">
      <h1>Fractal Web</h1>
      <p>WebAssembly module {wasmModule ? 'loaded' : 'loading...'}</p>

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