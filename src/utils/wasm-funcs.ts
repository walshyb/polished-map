export function processFile(arrayBuffer: ArrayBuffer, size: number, filename: string) {
  const filenamePtr: number = allocateUTF8(filename);
  // @ts-ignore
  window._processFile(arrayBuffer, size, filenamePtr);
}
