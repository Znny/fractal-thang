import './App.css'
import { useState, useEffect } from 'react'
import Fractal from './cpp/main'
import type { MainModule as FractalModule } from './cpp/main.d'


export default function App() {
  const [wasmModule, setWasmModule] = useState<FractalModule>()
  useEffect(() => {
    Fractal().then((instance) => {
      setWasmModule(instance)
      console.log(instance.getMessage(17))
    }
    ).catch((err) => {
      console.error('Failed to load WebAssembly module:', err)
    })

  }, [])

  return (
    <div className="app">
      <h1>Fractal Web</h1>
      <p>WebAssembly module {wasmModule?.getMessage(10) ? 'loaded' : 'loading...'}</p>
    </div>
  )
}