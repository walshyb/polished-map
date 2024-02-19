export function processFile(arrayBuffer: ArrayBuffer, size: number, filename: string): boolean {
  const filenamePtr: number = allocateUTF8(filename);
  const fn = UTF8ToString(filenamePtr);
  // @ts-ignore
  const bufferPtr: number = window._malloc(size);

  // Copy ArrayBuffer to wasm memory
  // @ts-ignore
  Module.HEAPU8.set(new Uint8Array(arrayBuffer), bufferPtr);

  // @ts-ignore
  const result: number = window._processFile(bufferPtr, size, filenamePtr);

  console.log('_processFile', result, fn);

  // @ts-ignore
  window._free(filenamePtr);
  // @ts-ignore
  window._free(bufferPtr);

  return Boolean(result);
}

export function getBlocks() {
  // @ts-ignore
  const result: number[] = window._getBlocks();
  // @ts-ignore
  window.blocks = result;
  console.log(result);
  return result;
}
