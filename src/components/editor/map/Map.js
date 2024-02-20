import Block from './Block';
import { getBlocks } from '../../../utils/wasm-funcs';
import { useAppSelector, useAppDispatch } from '../../../hooks';
import { setBlocks } from '../../../store/editorSlice';

export default function Map(
  { fileProcessed }: { fileProcessed: boolean }
) {

  const dispatch = useAppDispatch();
  //dispatch(setBlocks(blocksArray));

  // Get blocks from wasm
  if (fileProcessed) {
    console.log('calling getBlocks');
    // Load map
    const blocks = getBlocks();
  }
  // Draw blocks
  
  return (
    <div>
      <h1>Map</h1>
      <div>
      </div>
    </div>
  );
}
