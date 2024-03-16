import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App.tsx'
import {WASMProvider} from "./WasmProvider.tsx";
import './index.css'

ReactDOM.createRoot(document.getElementById('root')!).render(
    <React.StrictMode>
        <WASMProvider>
            <App/>
        </WASMProvider>
    </React.StrictMode>,
)
