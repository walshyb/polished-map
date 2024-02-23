import { createSlice } from "@reduxjs/toolkit";
import { getBlocks } from "../utils/wasm-funcs";

export interface Block {
  row: number;
  col: number;
  id: number;
}

interface Editor {
  blocks: Block[];
}

const initialState: Editor = {
  blocks: [],
};

export const editorSlice = createSlice({
  name: "editor",
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder
      .addCase("file/processFile/fulfilled", (state) => {
        // On file processing, fetch blocks
        state.blocks = getBlocks();
      })
      .addCase("file/openFileByName/fulfilled", (state) => {
        // On file processing, fetch blocks
        state.blocks = getBlocks();
      });
  },
});

export const {} = editorSlice.actions;
export default editorSlice.reducer;
