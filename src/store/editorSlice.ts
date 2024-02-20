import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from '../store/store'
import { getBlocks } from '../utils/wasm-funcs';

export interface Block {
  row: number;
  col: number;
  id: number;
}

interface Editor {
  blocks: Block[];
}

const initialState: Editor = {
  blocks: []
}

export const editorSlice = createSlice({
  name: 'editor',
  initialState,
  reducers: {
    setBlocks: (state) => {
      state.blocks = getBlocks();
    },
  },
  extraReducers: (builder) => {
    builder.addCase('file/processFile/fulfilled', (state, action) => {
      state.blocks = getBlocks();
    })
  }
})

export const { setBlocks } = editorSlice.actions;
export default editorSlice.reducer;
