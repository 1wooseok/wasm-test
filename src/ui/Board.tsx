import {useWASMContext} from "../WasmProvider.tsx";

export default function Board() {
    const wasmContext = useWASMContext();

    const boardPtr = wasmContext.get_board();

    const board: number[][] = [];
    const rowCount = wasmContext.get_row_count();
    const colCount = wasmContext.get_column_count();

    for (let i = 0; i < rowCount; ++i) {
        const row = [];
        for (let j = 0; j < colCount; ++j) {
            // Dereference the pointer and access the element manually
            const element = Module.HEAP32[(boardPtr + i * colCount * 4) / 4 + j];
            row.push(element);
        }
        board.push(row);
    }

    console.log(board)
    return (<div>{board.map((row, rowIndex) => {
        return <div>{
            row.map((el, colIndex) => <span key={`r-${rowIndex}.c-${colIndex}`}>{el}</span>)
        }</div>
    })}</div>)
}

