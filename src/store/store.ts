import { configureStore } from '@reduxjs/toolkit'
import { editorSlice } from './editorSlice'
import { fileSlice } from './fileSlice'

export const store = configureStore({
  reducer: {
    editor: editorSlice.reducer,
    file: fileSlice.reducer
  },
  devTools: true,
})

// Infer the `RootState` and `AppDispatch` types from the store itself
export type RootState = ReturnType<typeof store.getState>
// Inferred type: {posts: PostsState, comments: CommentsState, users: UsersState}
export type AppDispatch = typeof store.dispatch
