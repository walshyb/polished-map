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

interface TilesetReference {
  [index: number]: {
    constantName: string;
    symbolName: string;
  };
}

// Tileset name to tileset filename
interface TilesetLocation {
  // ex: TILESET_ECRUTEAK_SHRINE: "gfx/tilesets/johto_traditional.johto_common.2bpp"
  [tilesetName: string]: string;
}

interface MetatileImages {
  // block ID to base64 encoded png
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
  tilesetLocation: TilesetLocation;
}

const initialState: Editor = {
  blocks: [],
  activePalette: null,
  status: "initial",
  metatileset: {},
  mapData: {},
  tilesetLocation: {},
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
  async (_, { dispatch }) => {
    /**
     * Get tileset/location/palette data for every map (.ablk)
     *
     * ex:
     * map CeruleanPokeCenter1F, TILESET_POKECENTER, INDOOR, SIGN_BUILDING, CERULEAN_CITY, MUSIC_POKEMON_CENTER, 0, PALETTE_DAY
     */
    const mapsDataHandler = await getFileHandlerByPath("data/maps", "maps.asm");

    if (!mapsDataHandler) {
      throw new Error("Couldn't open maps data file");
    }

    const mapsData: File = await mapsDataHandler.getFile();
    const mapsDataFileContents: string = await mapsData.text();

    const mapLines: string[] = mapsDataFileContents.split("\n");

    const mapData: MapData = {};
    mapLines.forEach((line: string) => {
      // Skip if not relevant map data line
      if (!line.trim().startsWith("map ")) return;

      const tokens = line.split(" ");
      const mapName = tokens[1];

      mapData[mapName] = {
        name: mapName,
        tileset: tokens[2],
        environment: tokens[3],
        locationSign: tokens[4],
        location: tokens[5],
        palette: tokens[8],
      };
    });

    // TODO throw if no map data

    /**
     * Get tileset constant names, which will be used to map to .2bpp files
     *
     * ex:
     * 	const TILESET_JOHTO_MODERN         ; 02
     * 	const TILESET_JOHTO_OVERCAST       ; 03
     * 	const TILESET_BATTLE_TOWER_OUTSIDE ; 04
     * 	const TILESET_ECRUTEAK_SHRINE      ; 05
     */
    const tilesetConstantsHandler = await getFileHandlerByPath(
      "constants/",
      "tileset_constants.asm",
    );

    if (!tilesetConstantsHandler) {
      throw new Error("Couldn't open maps data file");
    }

    const constantsData: File = await tilesetConstantsHandler.getFile();
    const constantsBlob: string = await constantsData.text();

    const tilesetLines: string[] = constantsBlob.split("\n");

    const tilesetData: TilesetReference = {};
    let tilesetIndex = 2; // index starts at 2 because 1 is junk (ookattackbook!123)
    tilesetLines.forEach((line: string) => {
      // Skip if not relevant tileset constant line
      if (!line.trim().startsWith("const TILESET_")) return;
      const tokens = line.split(" ");
      const tilesetName = tokens[1];
      tilesetData[tilesetIndex++] = {
        constantName: tilesetName,
        symbolName: "",
      };
    });

    /**
     * Map tileset constants to tileset symbol names, then symbol
     *
     * ex:
     * 	tileset TilesetJohto1
     * 	tileset TilesetJohto2
     * 	tileset TilesetJohto3
     * 	tileset TilesetJohto4
     * 	tileset TilesetJohto5
     * 	.
     * 	.
     * 	.
     * 	TilesetJohto5GFX1:: INCBIN "gfx/tilesets/johto_traditional.johto_common.2bpp.vram0.lz"
     * 	TilesetKanto1GFX1:: INCBIN "gfx/tilesets/kanto.kanto_common.2bpp.lz"
     * 	TilesetKanto2GFX1:: INCBIN "gfx/tilesets/indigo_plateau.kanto_common.2bpp.lz"
     */
    const tilesetLocation: TilesetLocation = {};

    const symbolNamesHandler = await getFileHandlerByPath(
      "data/",
      "tilesets.asm",
    );

    if (!symbolNamesHandler) {
      console.log("thrown");
      throw new Error("Couldn't open maps data file");
    }

    const symbolsFile: File = await symbolNamesHandler.getFile();
    const symbolsBlob: string = await symbolsFile.text();

    const symbolsLines: string[] = symbolsBlob.split("\n");

    const symbolsData: any = {};
    let symbolIndex = 2;
    symbolsLines.forEach((line: string) => {
      // Skip if not relevant tileset constant line
      if (!line.trim().startsWith("tileset ") && !line.includes("GFX1::"))
        return;

      const tokens = line.split(" ");

      if (line.trim().startsWith("tileset ")) {
        const symbolName = tokens[1];
        console.log(symbolName);
        tilesetData[symbolIndex].symbolName = symbolName;
        symbolsData[symbolName] = tilesetData[symbolIndex++].constantName;
      }

      if (line.includes("GFX1::") && tokens.length > 1) {
        const symbolName = tokens[0].replace("GFX1::", "");
        const tilesetName = symbolsData[symbolName];
        console.log("GFX", symbolName, tilesetName, tokens[2], line);
        tilesetLocation[tilesetName] = tokens[2]
          .replace(".lz", "")
          .replace(".vram1", "")
          .replace(".vram0", "");
      }
    });

    return {
      mapData,
      tilesetLocation,
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
        state.mapData = action.payload.mapData;
        state.tilesetLocation = action.payload.tilesetLocation;
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
