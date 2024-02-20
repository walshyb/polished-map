import { createSlice, PayloadAction, createAsyncThunk } from '@reduxjs/toolkit'
import type { RootState } from '../store/store'
import { processFile as processFileUtil } from '../utils/wasm-funcs';

export interface File {
  name: string;
  size: number;
  active: boolean;
}

interface FileSlice {
  files: File[];
  state: string;
  error: string | null;
}

const initialState: FileSlice = {
  files: [],
  state: 'idle',
  error: null,
}

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
  }
})

/**
  * Selector func to get the active file
  */
export function getActiveFile(state: RootState): File | undefined {
  return state.file.files.find(file => file.active);
}
export const { setState } = fileSlice.actions;
export default fileSlice.reducer;

