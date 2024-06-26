import { getFileHandlerByPath } from "../helper-funcs";
import { processFile } from "../wasm-funcs";

/**
 * Fetches the default palette from the opened project and
 * loads it's contents into C++ map
 *
 * Default pokecrystal palette path:
 * gfx/tilesets/bg_tiles.pal
 */
export async function loadPalette(
  palettePath: string = "gfx/tilesets",
  fileName: string = "bg_tiles.pal",
): Promise<boolean> {
  const fileHandle: FileSystemFileHandle | undefined =
    await getFileHandlerByPath(palettePath, fileName);

  console.log("pal", palettePath, fileName);

  if (!fileHandle) {
    console.error("Couldn't open file");
    return false;
  }

  const file: File = await fileHandle.getFile();
  const arrayBuffer: ArrayBuffer = await file.arrayBuffer();

  console.log("pal call process file");

  const result: boolean = processFile(arrayBuffer, file.size, file.name);
  console.log("pal, load pal bool", result);

  return result;
}

export interface FileInfo {
  path: string;
  filename: string;
}
/**
 * Take a file path and split it into the path and filename
 */
export function splitFilePath(filePath: string): FileInfo {
  console.log(filePath);
  return {
    path: filePath.substring(0, filePath.lastIndexOf("/")),
    filename: filePath.substring(filePath.lastIndexOf("/") + 1),
  };
}
