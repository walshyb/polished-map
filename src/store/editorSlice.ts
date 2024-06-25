import { createSlice, createAsyncThunk } from "@reduxjs/toolkit";
import {
  getBlocks,
  loadTilesetData,
  processFile,
  getMetatilesetSize,
  getMetatileImage,
} from "../utils/wasm-funcs";
import { loadPalette } from "../utils/helpers/file-helper";
import { getFileHandlerByPath } from "../utils/helper-funcs";

export interface Block {
  row: number;
  col: number;
  id: number;
}

interface MetatileImages {
  // ID to base64 encoded png
  [key: string]: string;
}

interface MapData {
  [mapName: string]: {
    name: string;
    tileset: string;
    environment: string;
    locationSign: string;
    location: string;
    palette: string;
  };
}

interface Editor {
  blocks: Block[];
  activePalette?: string | null;
  status?: string;
  metatileset: MetatileImages;
  mapData: MapData;
}

const initialState: Editor = {
  blocks: [],
  activePalette: null,
  status: "initial",
  metatileset: {},
  mapData: {},
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
  async (data: any, { dispatch }) => {
    const { path, name } = data;

    /**
     * Process metatileset file
     */
    const handler = await getFileHandlerByPath(path, name);

    if (!handler) {
      throw new Error("Couldn't open metatile file");
    }

    const metatilesetFile: File = await handler.getFile();
    const metatilesetData = await metatilesetFile.arrayBuffer();

    // load metatileset
    const result = processFile(metatilesetData, metatilesetFile.size, name);

    if (!result) {
      // Caught by thunk
      throw new Error("Couldn't open metatileset");
    }

    /**
     * Render metatiles
     *
     * TODO: make this separate action?
     */

    dispatch(loadPaletteAction({}));
    const metatilesetSize: number = getMetatilesetSize();

    if (!metatilesetSize) {
      throw new Error("Metatileset size is 0");
    }

    for (let i = 0; i < metatilesetSize; i++) {
      //@ts-ignore
      const metatileImageEncoded: string = getMetatileImage(i);
      dispatch({
        type: "saveMetatileImage",
        payload: {
          id: i,
          image: metatileImageEncoded,
        },
      });
    }

    return {
      activeMetatileset: name,
    };
  },
);

export const loadMapDataAction = createAsyncThunk(
  "editor/loadMapData",
  async () => {
    const mapsDataHandler = await getFileHandlerByPath("data/maps", "maps.asm");

    if (!mapsDataHandler) {
      throw new Error("Couldn't open maps data file");
    }

    const mapsData: File = await mapsDataHandler.getFile();
    const mapsDataFileContents: string = await mapsData.text();

    const lines: string[] = mapsDataFileContents.split("\n");

    const data: MapData = {};
    lines.forEach((line: string) => {
      // Skip if not relevant map data line
      if (!line.trim().startsWith("map ")) return;

      const tokens = line.split(" ");
      const mapName = tokens[1];

      data[mapName] = {
        name: mapName,
        tileset: tokens[2],
        environment: tokens[3],
        locationSign: tokens[4],
        location: tokens[5],
        palette: tokens[8],
      };
    });

    // TODO throw if no map data

    return {
      data,
    };
  },
);

export const loadTilesetAction = createAsyncThunk(
  "editor/loadTileset",
  async (data: any, { dispatch }) => {
    const { path, name } = data;
    const tilesetHandler = await getFileHandlerByPath(path, name);
    const beforeTilesetHandler = await getFileHandlerByPath(
      path,
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
      name,
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
      .addCase(loadMapDataAction.fulfilled, (state, action) => {
        state.mapData = action.payload.data;
      })
      .addCase(loadPaletteAction.fulfilled, (state, action) => {
        state.activePalette = action.payload.activePalette;
      })
      .addCase("saveMetatileImage", (state, action: any) => {
        state.metatileset[action.payload.id] = action.payload.image;
      })
      .addCase(loadPaletteAction.rejected, (state, action) => {
        state.status = "Couldn't load palette";
      });
  },
});

export const {} = editorSlice.actions;
export default editorSlice.reducer;
