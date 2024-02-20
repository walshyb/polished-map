import type { FileNode } from '../store/fileSlice';

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

const validFileTypes = ['.ablk', '.blk', '.map', '.tileset', '.metatileset', '.tilemap', '.bin', '.asm'];
// Create a tree of files
// We'll lazily fetch contents as needed
export async function readFilesInDirectory(directoryHandler: FileSystemDirectoryHandle, recursive: boolean = false): Promise<FileNode[]> {
  const entries: AsyncIterableIterator<[string, FileSystemDirectoryHandle | FileSystemFileHandle]> = directoryHandler.entries();

  let result: FileNode[] = [];

  // loop over entries iterator
  for await (const [filename, currentNode] of entries) {

    let fileNode: FileNode = {
      name: filename,
      isFile: currentNode.kind === 'file',
      active: false,
    };

    if (recursive && currentNode.kind === 'directory') {
      // If it's a directory, recursively call readFilesInDirectory
      const directoryResult = await readFilesInDirectory(currentNode, recursive);
      // Append the result of the recursive call to the current currentNode
      fileNode.children = directoryResult;
    }
  
    // Directory is valid if it has directory
    const validDirectory = currentNode.kind === 'directory' && fileNode.children?.length;

    // File is valid is it's extension is in validFileTypes
    const extension: string | null = filename.substring(filename.lastIndexOf('.'));
    const validFile = currentNode.kind === 'file' && extension && validFileTypes.includes(extension);

    // Only save directories and valid files
    if (validFile || validDirectory){
      result.push(fileNode);
    }
  }
  

  return result;
}
