import './App.css'
import WebGLCanvas from './components/WebGLCanvas'

function App() {
  return (
    <div className="app-container">
      <h1>Fractal Renderer</h1>
      <WebGLCanvas width={800} height={600} />
    </div>
  )
}

export default App
