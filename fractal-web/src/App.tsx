import './App.css'
import { useState, useEffect, useRef } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule, TriangleRenderer, Window } from './cpp/main.d'

export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  const [window, setWindow] = useState<Window>()
  const [renderer, setRenderer] = useState<TriangleRenderer>()
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
    }
    ).catch((err) => {
      console.error('Failed to load WebAssembly module:', err)
    })
    console.log('WebAssembly module loaded')
  }, [])

  useEffect(() => {
    if (window && renderer && canvasRef.current) {
      console.log('Initializing OpenGL context...')
      const success = window.init('#'+canvasRef.current.id)
      if (success) {
        console.log('OpenGL context initialized successfully')
        // Set canvas size
        const canvas = canvasRef.current
        canvas.width = window.getWidth()
        canvas.height = window.getHeight()
        // Initial render
        handleRender()
      } else {
        console.error('Failed to initialize OpenGL context')
      }
    }
  }, [renderer])

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