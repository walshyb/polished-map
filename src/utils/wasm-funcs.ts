import { Block } from "../store/editorSlice";

/**
 * Takes array buffer and passes it the FileProcessor in C++.
 *
 * @return {boolean} - Whether the file was processed successfully
 */
export function processFile(
  arrayBuffer: ArrayBuffer,
  size: number,
  filename: string,
): boolean {
  const filenamePtr: number = allocateUTF8(filename);
  // @ts-ignore
  const bufferPtr: number = window._malloc(size);

  // Converts binary data of arrayBuffer to byte-separated array
  // Copy file contents to wasm memory
  // @ts-ignore
  Module.HEAPU8.set(new Uint8Array(arrayBuffer), bufferPtr);

  const success: boolean = Boolean(
    // @ts-ignore
    window._processFile(bufferPtr, size, filenamePtr),
  );

  // @ts-ignore
  window._free(filenamePtr);
  // @ts-ignore
  window._free(bufferPtr);

  return success;
}

/**
 * Get the Blocks to draw from wasm.
 * These Blocks represent the layout of the tilemap
 */
export function getBlocks(): Block[] {
  // Get pointer to blocks array from Map in C++
  // @ts-ignore
  const blocksArrayPtr: number = window._getBlocks();

  // Num blocks is the size of the file in bytes
  // TODO: hardcoded. find out length
  const blocksArrayLength: number = 360;
  let blocksArray: Block[] = [];

  // Access block data from memory
  for (let i = 0; i < blocksArrayLength; i++) {
    // @ts-ignore
    // Get pointer to block in the array in memory
    const blockPointer = Module.HEAP32[blocksArrayPtr / 4 + i]; // Assuming 32-bit integers

    // @ts-ignore
    const blockRow = Module.HEAPU8[blockPointer + 0];
    // @ts-ignore
    const blockCol = Module.HEAPU8[blockPointer + 1];
    // @ts-ignore
    const blockId = Module.HEAPU8[blockPointer + 2];

    blocksArray.push({
      row: blockRow,
      col: blockCol,
      id: blockId,
    } as Block);
  }

  return blocksArray;
}

export function loadTilesetData(
  tilesetBuffer: ArrayBuffer,
  tilesetSize: number,
  beforeTilesetBuffer: ArrayBuffer,
  beforeTilesetSize: number,
  filename: string,
): boolean {
  const filenamePtr: number = allocateUTF8(filename);

  // @ts-ignore
  const tilesetBufferPtr: number = window._malloc(tilesetSize);
  // @ts-ignore
  const beforeTilesetBufferPtr: number = window._malloc(beforeTilesetSize);

  // @ts-ignore
  Module.HEAPU8.set(new Uint8Array(tilesetBuffer), tilesetBufferPtr);

  // @ts-ignore
  Module.HEAPU8.set(
    new Uint8Array(beforeTilesetBuffer),
    beforeTilesetBufferPtr,
  );

  const success: boolean = Boolean(
    // @ts-ignore
    window._readMetatileData(
      tilesetBufferPtr,
      tilesetSize,
      filenamePtr,
      beforeTilesetBufferPtr,
      beforeTilesetSize,
    ),
  );

  return false;
}
