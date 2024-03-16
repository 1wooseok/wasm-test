import {useEffect} from "react";

export default function useWASM() {
    let wasm;

    useEffect(() => {
        async function fetchWASM() {
            try {
                wasm = {
                    get_board: Module.cwrap("get_board", "number", []),
                    init_game: Module.cwrap("init_game", null, []),
                    get_row_count: Module.cwrap("get_row_count", "number", []),
                    get_column_count: Module.cwrap("get_column_count", "number", []),
                    get_score: Module.cwrap("get_score", "number", ["number"]),
                    get_color: Module.cwrap("get_color", "number", ["number", "number"]),
                    is_placeable: Module.cwrap("is_placeable", "number", ["number", "number"]),
                    place_stone: Module.cwrap("place_stone", "number", ["number", "number", "number"]),
                    insert_row: Module.cwrap("insert_row", "number", ["number", "number"]),
                    insert_column: Module.cwrap("insert_column", "number", ["number", "number"]),
                    remove_row: Module.cwrap("remove_row", "number", ["number", "number"]),
                    remove_column: Module.cwrap("remove_column", "number", ["number", "number"]),
                    swap_rows: Module.cwrap("swap_rows", "number", ["number", "number", "number"]),
                    swap_columns: Module.cwrap("swap_columns", "number", ["number", "number", "number"]),
                    copy_row: Module.cwrap("copy_row", "number", ["number", "number", "number"]),
                    copy_column: Module.cwrap("copy_column", "number", ["number", "number", "number"]),
                    calc_score: Module.cwrap("calc_score", "number", ["number", "number", "number"]),
                    add_score_by_color: Module.cwrap("add_score_by_color", null, ["number", "number"]),
                    reduce_score_by_color: Module.cwrap("reduce_score_by_color", null, ["number", "number"]),
                    is_valid_row_col: Module.cwrap("is_valid_row_col", "number", ["number", "number"])
                };

                Module.onRuntimeInitialized = async function() {
                    await wasm.init_game();

                    const x = await wasm.get_row_count();
                    console.log(x);

                    const boardPtr = await wasm.get_board();

                    const board = [];
                    const rowCount = await wasm.get_row_count();
                    const colCount = await wasm.get_column_count();
                    for (let i = 0; i < rowCount; ++i) {
                        const row = [];
                        for (let j = 0; j < colCount; ++j) {
                            // Dereference the pointer and access the element manually
                            const element = Module.HEAP32[(boardPtr + i * colCount * 4) / 4 + j];
                            row.push(element);
                        }
                        board.push(row);
                    }

                    // Log the array
                    console.log(board);
                };
            } catch (error) {
                console.error(`Error while fetching .wasm ${error}`);
            }
        }

        fetchWASM()
    }, []);

    return wasm;
}
