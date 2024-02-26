import { createSlice, createAsyncThunk } from "@reduxjs/toolkit";
import { getBlocks, loadTilesetData, processFile } from "../utils/wasm-funcs";
import { loadPalette } from "../utils/helpers/file-helper";
import { getFileHandlerByPath } from "../utils/helper-funcs";

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

export const loadMetatilesetAction = createAsyncThunk(
  "editor/loadMetatileset",
  async (data: any) => {
    const { path, name } = data;

    const handler = await getFileHandlerByPath(
      "data/tilesets/",
      "johto_overcast_metatiles.bin",
    );

    if (!handler) {
      throw new Error("Couldn't open metatile file");
    }

    const metatilesetFile: File = await handler.getFile();
    const metatilesetData = await metatilesetFile.arrayBuffer();

    // load metatileset
    const result = processFile(
      metatilesetData,
      metatilesetFile.size,
      "johto_overcast_metatiles.bin",
    );

    if (!result) {
      // Caught by thunk
      throw new Error("Couldn't open metatileset");
    }

    return {
      activeMetatileset: name,
    };
  },
);

export const loadTilesetAction = createAsyncThunk(
  "editor/loadTileset",
  async (data: any) => {
    const { path, name } = data;
    const tilesetHandler = await getFileHandlerByPath(
      "gfx/tilesets/",
      "johto_overcast.johto_common.2bpp",
    );
    const beforeTilesetHandler = await getFileHandlerByPath(
      "gfx/tilesets/",
      "johto_common.2bpp",
    );

    if (!tilesetHandler || !beforeTilesetHandler) {
      // TODO make these states predefined and reuseable
      // caught by thunk
      throw new Error("Couldn't open file");
    }

    const tileset: File = await tilesetHandler.getFile();
    const tilesetArrayBuffer = await tileset.arrayBuffer();

    const beforeTileset: File = await beforeTilesetHandler.getFile();
    const beforeTilesetArrayBuffer = await beforeTileset.arrayBuffer();

    const result = loadTilesetData(
      tilesetArrayBuffer,
      tileset.size,
      beforeTilesetArrayBuffer,
      beforeTileset.size,
      name || "johto_overcast.johto_common.2bpp",
    );

    if (!result) {
      throw new Error("Tileset opening failed");
    }

    return {
      activeTileset: name,
    };
  },
);

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
