import type { FileNode } from "../store/fileSlice";
import FileHandlerManager from "../store/fileManagerSingleton";

interface Size {
  width: number;
  height: number;
}
// TODO: put in map helpers file?
export function calculateMapSizes(size: number): Size {
  // Idk, we might want to save this
  const validSizes: Size[] = [];

  for (let width = size; width >= 1; width--) {
    const height = Math.floor(size / width);

    if (width * height === size) {
      validSizes.push({ width, height });
    }
  }

  // Prefer size multiples of 10x9, the typical unit size
  return validSizes[Math.floor((validSizes.length - 1) / 2)];
}

const validFileTypes = [
  ".ablk",
  ".blk",
  ".map",
  ".tileset",
  ".metatileset",
  ".tilemap",
  ".bin",
];
// Need to ignore directories that don't contain useful files,
// particularly because they can be large dirs and filesystem api hangs
const ignoreDirs = ["pokemon", "audio", "whirlpool", "gfx"];
// Create a tree of files
// We'll lazily fetch contents as needed
export async function readFilesInDirectory(
  directoryHandler: FileSystemDirectoryHandle,
  currentDirectory: string,
  recursive: boolean = false,
): Promise<FileNode[]> {
  const entries: AsyncIterableIterator<
    [string, FileSystemDirectoryHandle | FileSystemFileHandle]
  > = directoryHandler.entries();

  let result: FileNode[] = [];

  // loop over entries iterator
  for await (const [filename, currentNode] of entries) {
    let fileNode: FileNode = {
      name: filename,
      isFile: currentNode.kind === "file",
      active: false,
      path: currentDirectory + "/",
    };

    if (
      recursive &&
      currentNode.kind === "directory" &&
      !ignoreDirs.includes(currentNode.name)
    ) {
      const directoryResult: FileNode[] = await readFilesInDirectory(
        currentNode,
        `${currentDirectory}/${currentNode.name}`,
        recursive,
      );
      // If it's a directory, recursively call readFilesInDirectory
      const sorted = directoryResult.sort((a, b) =>
        a.name.localeCompare(b.name),
      );
      // Append the result of the recursive call to the current currentNode
      fileNode.children = sorted;
    }

    // Directory is valid if it has directory
    const validDirectory =
      currentNode.kind === "directory" && fileNode.children?.length;

    // File is valid is it's extension is in validFileTypes
    const extension: string | null = filename.substring(
      filename.lastIndexOf("."),
    );
    const validFile =
      currentNode.kind === "file" &&
      extension &&
      validFileTypes.includes(extension);

    // Only save directories and valid files
    if (validFile || validDirectory) {
      result.push(fileNode);
    }
  }

  return result;
}

export async function getFileHandlerByPath(
  path: string,
  filename: string = "",
): Promise<FileSystemFileHandle | undefined> {
  const fullPath = path + "/" + filename;
  const fileHandlerManager = FileHandlerManager.getInstance();

  // @ts-ignore Because guaranteed to get directory handle and not file handle
  let directoryHandler: FileSystemDirectoryHandle =
    fileHandlerManager.getFileHandler("root");

  if (!directoryHandler) {
    console.log("No root directory");
    return;
  }

  let fileHandler: FileSystemFileHandle;
  try {
    let splitPath: string[] = path.split("/");

    // Go to each directory one by one in the path (only way to do it)
    for (const dirName of splitPath) {
      // Handle trailing /
      if (dirName === "") continue;
      directoryHandler = await directoryHandler.getDirectoryHandle(dirName);
    }

    // Now we're at the folder of the file we're looking for
    fileHandler = await directoryHandler.getFileHandle(filename);
  } catch (error: any | DOMException) {
    console.log("Couldn't open file or directory.");
    return;
  }

  if (!fileHandler) {
    console.log("No file handler");
    return;
  }

  fileHandlerManager.addFileHandler(fullPath, fileHandler);
  return fileHandler;
}
