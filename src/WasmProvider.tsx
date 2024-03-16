import {createContext, useContext, useEffect, useState} from "react";

const WASMContext = createContext(undefined);

// 건물 정보 context - 이름, 자립률 등
export const WASMProvider = ({children}: {children: React.ReactNode}) => {
    const wasm = useWASM();

    if (!wasm) {
        return null;
    }

    return (
        <WASMContext.Provider value={wasm}>
            {children}
        </WASMContext.Provider>
    );
};

export const useWASMContext = () => {
    const context = useContext(WASMContext);

    if (!context) {
        throw "Context 값이 전달되지 않았습니다. Provider 바깥에 있는 컴포넌트인지 확인해주세요";
    }

    return context;
};


function useWASM() {
    const [wasmModule, setWasmModule] = useState()

    useEffect(() => {
        async function fetchWASM() {
            try {
                const wasm = {
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
                } as const;

                setWasmModule(wasm);
            } catch (error) {
                console.error(`Error while fetching .wasm ${error}`);
            }
        }

        fetchWASM();
    }, []);

    return wasmModule;
}
