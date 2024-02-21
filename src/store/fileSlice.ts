import { createSlice, PayloadAction, createAsyncThunk } from '@reduxjs/toolkit'
import type { RootState } from '../store/store'
import { processFile as processFileUtil } from '../utils/wasm-funcs';
import { readFilesInDirectory, getFileHandlerByPath } from '../utils/helper-funcs';
import FileHandlerManager from './fileManagerSingleton';

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
  files: [],  // open files
  state: 'idle',
  error: null,
}

export const openFileByName = createAsyncThunk('file/openFileByName', async (data: any, { dispatch }) => {
  const { path, name } = data;
  const fileHandler = await getFileHandlerByPath(path, name);

  if (!fileHandler) {
    // TODO make these states predefined and reuseable
    return {
      state: 'error',
      error: 'Couldn\'t open file'
    }
  }

  const file: File = await fileHandler.getFile();
  const size = await file.size;
  const arrayBuffer = await file.arrayBuffer();

  const result: boolean = processFileUtil(arrayBuffer, file.size, file.name);

  // TODO:
  // return result with information from C++
  return {
    result,
    filename: file.name,
    size: file.size,
    active: true,
    error: ''
  };
});

// TODO:
// go through files and make sure that this is a valid project
/**
  * Opens directory specified by user, and stores references to tile-related files
  */
export const openProject = createAsyncThunk('file/openProject', async () => {
  try {
    // Prompt user to allow access to directory
    const directoryHandler: FileSystemDirectoryHandle = await window.showDirectoryPicker();
    const directoryName: string = directoryHandler.name;

    // Get all files and subdirectories
    const fileTree: FileNode[] = await readFilesInDirectory(directoryHandler, directoryName, true);

    // Add root directory handler to handlermanager singleton storage
    const fileHandlerManager = FileHandlerManager.getInstance();
    fileHandlerManager.addFileHandler('root', directoryHandler);

    return {
      fileTree,
    };
  } catch (error: any | DOMException) {
    console.log(error);
    return {
      error: 'User did not grant file access',
    };
  }
});

// TODO rename to openFile?
export const processFile = createAsyncThunk('file/processFile', async (data: any, { dispatch }) => {
  const result: boolean = processFileUtil(data.arrayBuffer, data.size, data.filename);

  // TODO:
  // return result with information from C++
  return {
    result,
    filename: data.filename,
    size: data.size
  };
});

export const fileSlice = createSlice({
  name: 'editor',
  initialState,
  reducers: {
    setState: (state, action: PayloadAction<string>) => {
      state.state = action.payload;
    },
    toggleExplorer: (state) => {
      state.explorerOpen = !state.explorerOpen;
    }
  },
  extraReducers: (builder) => {
    builder
      .addCase(processFile.pending, (state) => {
        state.state = 'loading';
      })
      .addCase(processFile.fulfilled, (state, action) => {
        if (action.payload.result) {
          const activeFile: FileNode  = {
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true
          }
          state.files.push(activeFile);
          state.state = 'idle';
          state.error = null;
        } else {
          state.state = 'error';
          state.error = 'File processing failed';
        }
      })
      .addCase(processFile.rejected, (state, action) => {
        state.state = 'error';
        state.error = 'File processing failed';
      })
      .addCase(openProject.fulfilled, (state, action) => {
        if (action.payload.error) {
          state.state = 'error';
          state.error = action.payload.error;
        } else {
          state.files = action.payload.fileTree || [];
          state.explorerOpen = true;
        }
      })
      .addCase(openFileByName.fulfilled, (state, action) => {
        if (action.payload.result) {
          state.state = 'processed';
          state.activeFile = {
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true
          };
        } else {
          state.state = 'error';
          state.error = action.payload.error;
        }
      })
  }
})

export const { setState, toggleExplorer } = fileSlice.actions;
export default fileSlice.reducer;

