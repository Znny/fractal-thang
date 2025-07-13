import './App.css'
import { useState, useEffect, useRef } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule, Renderer, Window } from './cpp/main.d'

export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  const [window, setWindow] = useState<Window>()
  const [renderer, setRenderer] = useState<Renderer>()
  const [clickCount, setClickCount] = useState(0)
  const canvasRef = useRef<HTMLCanvasElement>(null)

  useEffect(() => {
    console.log('Loading WebAssembly module')
    Fractal().then((instance) => {
      setWasmModule(instance)
      
      // Create renderer instance
      const rendererInstance = new instance.Renderer()
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
        renderer.render()
      } else {
        console.error('Failed to initialize OpenGL context')
      }
    }
  }, [renderer])

  useEffect(() => {
    console.log('clickCount', clickCount)
  }, [clickCount])

  const handleRender = () => {
    if (renderer) {
      renderer.render()
    }
  }

  const handleResize = () => {
    if (renderer && canvasRef.current) {
      const canvas = canvasRef.current
      const rect = canvas.getBoundingClientRect()
      window.Resize(rect.width, rect.height)
      canvas.width = rect.width
      canvas.height = rect.height
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

      <div style={{ marginTop: '20px' }}>
        <button onClick={handleRender}>
          Render Frame
        </button>
        <button onClick={handleResize}>
          Resize Canvas
        </button>
        <button onClick={() => {
          console.log('button clicked')
          setClickCount(clickCount => clickCount + 1)
        }}>
          Increment Click Count
        </button>
      </div>
    </div>
  )
}