import { createSlice, createAsyncThunk } from "@reduxjs/toolkit";
import { getBlocks } from "../utils/wasm-funcs";
import { loadPalette } from "../utils/helpers/file-helper";

export interface Block {
  row: number;
  col: number;
  id: number;
}

interface Editor {
  blocks: Block[];
  activePalette?: string | null;
  status?: string;
}

const initialState: Editor = {
  blocks: [],
  activePalette: null,
  status: "initial",
};

interface LoadPaletteResult {
  activePalette: string;
  error?: any;
}
interface LoadPaletteData {
  path?: string;
  filename?: string;
}
export const loadPaletteAction = createAsyncThunk<
  LoadPaletteResult,
  LoadPaletteData
>("editor/loadPalette", async (data) => {
  const { path, filename } = data;

  const result: boolean = await loadPalette(path, filename);

  if (!result) {
    // Caught by thunk
    throw new Error("Error loading palette.");
  }

  return {
    activePalette: filename || "default",
  };
});

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
      })
      .addCase(loadPaletteAction.fulfilled, (state, action) => {
        state.activePalette = action.payload.activePalette;
      })
      .addCase(loadPaletteAction.rejected, (state, action) => {
        state.status = "Couldn't load palette";
      });
  },
});

export const {} = editorSlice.actions;
export default editorSlice.reducer;
