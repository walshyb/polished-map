import { createSlice, PayloadAction, createAsyncThunk } from '@reduxjs/toolkit'
import type { RootState } from '../store/store'
import { processFile as processFileUtil } from '../utils/wasm-funcs';
import { readFilesInDirectory, getFileHandlerByPath } from '../utils/helper-funcs';
import FileHandlerManager from './fileManagerSingleton';

export interface FileNode {
  name: string;
  size?: number;
  active: boolean;
  isFile: boolean;
  children?: FileNode[];
  isOpen?: boolean;
  path?: string;
}

interface FileSlice {
  files: FileNode[];
  state: string;
  error: string | null;
}

const initialState: FileSlice = {
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
  },
  extraReducers: (builder) => {
    builder
      .addCase(processFile.pending, (state) => {
        state.state = 'loading';
      })
      .addCase(processFile.fulfilled, (state, action) => {
        if (action.payload.result) {
          state.files.push({
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true
          });
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
        }
      })
      .addCase(openFileByName.fulfilled, (state, action) => {
        if (action.payload.result) {
          state.state = 'processed';
          state.files.push({
            isFile: true,
            name: action.payload.filename,
            size: action.payload.size,
            active: true
          });
        } else {
          state.state = 'error';
          state.error = action.payload.error;
        }
      })
  }
})

/**
  * Selector func to get the active file
  */
export function getActiveFile(state: RootState): FileNode | undefined {
  // TODO: support nested files
  return state.file.files.find(file => file.active);
}
export const { setState } = fileSlice.actions;
export default fileSlice.reducer;

