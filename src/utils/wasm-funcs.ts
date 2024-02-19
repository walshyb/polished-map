export function processFile(arrayBuffer: ArrayBuffer, size: number, filename: string) {
  const filenamePtr: number = allocateUTF8(filename);
  // @ts-ignore
  const bufferPtr: number = window._malloc(size);

  // Copy ArrayBuffer to wasm memory
  // @ts-ignore
  Module.HEAPU8.set(new Uint8Array(arrayBuffer), bufferPtr);

  // @ts-ignore
  window.__ZN13FileProcessor11processFileEPKhmPKc(bufferPtr, size, filenamePtr);

  // @ts-ignore
  window._free(filenamePtr);
  // @ts-ignore
  window._free(bufferPtr);
}
