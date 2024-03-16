import {useWASMContext} from "./WasmProvider.tsx";
import Board from "./ui/Board.tsx";

export default function App() {
  const wasmContext = useWASMContext();

  wasmContext.init_game();

  return (
    <div>
      <Board />
    </div>
  )
}

