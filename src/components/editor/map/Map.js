import Block from './Block';
import { getBlocks } from '../../../utils/wasm-funcs';

export default function Map(
  { fileProcessed }: { fileProcessed: boolean }
) {
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
