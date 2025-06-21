import './App.css'
import { useState, useEffect } from 'react'
import Fractal from './cpp/main'


export default function App() {
  const [wasmModule, setWasmModule] = useState<any>(null)
  useEffect(() => {
    Fractal().then((instance) => {
      setWasmModule(instance)
    }
    ).catch((err) => {
      console.error('Failed to load WebAssembly module:', err)
    })

  }, [])

  return (
    <div className="app">
      <h1>Fractal Web</h1>
      <p>WebAssembly module {wasmModule ? 'loaded' : 'loading...'}</p>
    </div>
  )
}