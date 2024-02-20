import { createSlice, PayloadAction, createAsyncThunk } from '@reduxjs/toolkit'
import type { RootState } from '../store/store'
import { processFile as processFileUtil } from '../utils/wasm-funcs';

export interface File {
  name: string;
  size: number;
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
  if (result) {
    dispatch(setState('processed'));
  }
});

export const fileSlice = createSlice({
  name: 'editor',
  initialState,
  reducers: {
    setState: (state, action: PayloadAction<string>) => {
      state.state = action.payload;
    },
  }
})

export const { setState } = fileSlice.actions;
export default fileSlice.reducer;

