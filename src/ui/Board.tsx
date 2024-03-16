import {useWASMContext} from "../WasmProvider.tsx";
import {useEffect, useState} from "react";

enum EColor {
    BLACK,
    WHITE
}

function mappingStyleByColor(color: EColor): string {
    switch (color) {
        case EColor.BLACK:
            return "blue";
        case EColor.WHITE:
            return "red"
        default:
            return "inherit"
    }
}

export default function Board() {
    const wasmContext = useWASMContext();
    const [currColor, setCurrColor] = useState<EColor>(EColor.BLACK);
    const [board, setBoard] = useState<number[][]>([]);

    useEffect(() => {
        const boardPtr = wasmContext.get_board();

        const tmpBoard: number[][] = [];
        const rowCount = wasmContext.get_row_count();
        const colCount = wasmContext.get_column_count();

        for (let i = 0; i < rowCount; ++i) {
            const row = [];
            for (let j = 0; j < colCount; ++j) {
                // Dereference the pointer and access the element manually
                const element = Module.HEAP32[(boardPtr + i * colCount * 4) / 4 + j];
                row.push(element);
            }
            tmpBoard.push(row);
        }

        setBoard(tmpBoard)
    }, [currColor]);


    console.log(board)
    function handlePlaceStone(rowIndex: number, colIndex: number) {
        console.log({rowIndex, colIndex})
        if (!wasmContext.is_placeable(rowIndex, colIndex)) {
            return;
        }

        const isOk = wasmContext.place_stone(currColor, rowIndex, colIndex);
        console.log({isOk})
        if (!isOk) {
            console.error("place fail.");
        }


        const nextTurn = currColor === EColor.BLACK ? EColor.WHITE : EColor.BLACK
        console.log({ currTurn: currColor, nextTurn });
        setCurrColor(nextTurn);

    }

    return (<div className={"w-[600px] h-[600px]"}>{board.map((row, rowIndex) => {
        return <div>{
            row.map((el, colIndex) => <button className={"w-[30px] h-[30px] bg-black mx-[2px]"}  onClick={() => handlePlaceStone(rowIndex, colIndex)} key={`r-${rowIndex}.c-${colIndex}`}>
                <span  style={{
                    width: "20px",
                    height: "20px",
                    // background: mappingStyleByColor(el),
                    background: "black",
                }} />
            </button>)
        }</div>
    })}</div>)
}

