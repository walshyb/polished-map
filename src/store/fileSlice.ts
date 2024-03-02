import { createSlice, PayloadAction, createAsyncThunk } from "@reduxjs/toolkit";
import { processFile as processFileUtil } from "../utils/wasm-funcs";
import {
  readFilesInDirectory,
  getFileHandlerByPath,
} from "../utils/helper-funcs";
import FileHandlerManager from "./fileManagerSingleton";
import {
  loadPaletteAction,
  loadMetatilesetAction,
  loadTilesetAction,
} from "./editorSlice";

export interface FileNode {
  name: string;
  size?: number;
  active: boolean; // TODO eventually have this represent a tab ID
  isFile: boolean;
  children?: FileNode[];
  isOpen?: boolean;
  path?: string;
}

interface FileSlice {
  explorerOpen: boolean;
  files: FileNode[];
  state: string;
  error: string | null;
  activeFile: FileNode | null;
}

const initialState: FileSlice = {
  explorerOpen: false,
  activeFile: null,
  files: [], // open files
  state: "idle",
  error: null,
};

/**
 * This is really open map file and load related files
 */
export const openFileByName = createAsyncThunk(
  "file/openFileByName",
  async (data: any, { dispatch }) => {
    const { path, name } = data;
    const fileHandler = await getFileHandlerByPath(path, name);

    if (!fileHandler) {
      // TODO make these states predefined and reuseable
      // caught by thunk
      throw new Error("Couldn't open file");
    }

    const file: File = await fileHandler.getFile();
    //const size = file.size;
    const arrayBuffer = await file.arrayBuffer();

    // TODO: go through directory and look for related files

    // Process map file
    // TODO name this processMapFile (i..e AzaleaTown.ablk)
    const result: boolean = processFileUtil(arrayBuffer, file.size, file.name);

    // load tileset: gfx/tilesets/johto_overcast.johto_common.png
    // load tileset before: gfx/tilesets/johto_common.png
    // load metatileset: data/tilesets/johto_overcast_metatiles.bin

    if (!result) {
      throw new Error("Map (.ablk) opening failed");
    }

    dispatch(
      loadMetatilesetAction({
        path: "data/tilesets/",
        name: "johto_overcast_metatiles.bin",
      }),
    );

    // TODO:
    // return result with information from C++
    return {
      result,
      filename: file.name,
      size: file.size,
      active: true,
      error: "",
    };
  },
);

// TODO:
// go through files and make sure that this is a valid project
/**
 * Opens directory specified by user, and stores references to tile-related files
 */
export const openProject = createAsyncThunk(
  "file/openProject",
  async (_, { dispatch }) => {
    // Prompt user to allow access to directory
    // Errors caught by thunk
    const directoryHandler: FileSystemDirectoryHandle =
      await window.showDirectoryPicker();

    const fileTree: FileNode[] = await readFilesInDirectory(
      directoryHandler,
      "",
      true,
    );

    // Add root directory handler to handlermanager singleton storage
    const fileHandlerManager = FileHandlerManager.getInstance();
    fileHandlerManager.addFileHandler("root", directoryHandler);

    // Load default palette

    dispatch(
      loadTilesetAction({
        path: "gfx/tilesets/",
        name: "johto_overcast.johto_common.2bpp",
      }),
    );
    dispatch(loadPaletteAction({}));

    return {
      fileTree,
    };
  },
);

// TODO rename to openFile?
export const processFile = createAsyncThunk(
  "file/processFile",
  async (data: any) => {
    const result: boolean = processFileUtil(
      data.arrayBuffer,
      data.size,
      data.filename,
    );

    // TODO:
    // return result with information from C++
    return {
      result,
      filename: data.filename,
      size: data.size,
    };
  },
);

export const fileSlice = createSlice({
  name: "editor",
  initialState,
  reducers: {
    setState: (state, action: PayloadAction<string>) => {
      state.state = action.payload;
    },
    toggleExplorer: (state) => {
      state.explorerOpen = !state.explorerOpen;
    },
  },
  extraReducers: (builder) => {
    builder
      .addCase(processFile.pending, (state) => {
        state.state = "loading";
      })
      .addCase(processFile.fulfilled, (state, action) => {
        if (action.payload.result) {
          const activeFile: FileNode = {
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true,
          };
          state.files.push(activeFile);
          state.state = "idle";
          state.error = null;
        } else {
          state.state = "error";
          state.error = "File processing failed";
        }
      })
      .addCase(processFile.rejected, (state) => {
        state.state = "error";
        state.error = "File processing failed";
      })
      .addCase(openProject.fulfilled, (state, action) => {
        state.files = action.payload.fileTree || [];
        state.explorerOpen = true;
      })
      .addCase(openProject.rejected, (state, action) => {
        state.state = "error";

        // TODO: handle specific error messages
        state.error = action.error.message || "";
      })
      .addCase(openFileByName.fulfilled, (state, action) => {
        if (action.payload.result) {
          state.state = "processed";
          state.activeFile = {
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true,
          };
        } else {
          state.state = "error";
          state.error = action.payload.error;
        }
      });
  },
});

export const { setState, toggleExplorer } = fileSlice.actions;
export default fileSlice.reducer;
