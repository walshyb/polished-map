type Handler = FileSystemFileHandle | FileSystemDirectoryHandle;

// Singleton class to manage file handlers
export default class FileHandlerManager {
  // Holds the instance
  private static instance: FileHandlerManager;

  // Need 1 file handler per active file
  // Useful when file tabs are implemented
  private fileHandlers: Map<string, Handler>; // Use Map to store file handler objects

  private constructor() {
    this.fileHandlers = new Map<string, Handler>();
  }

  public static getInstance(): FileHandlerManager {
    if (!FileHandlerManager.instance) {
      FileHandlerManager.instance = new FileHandlerManager();
    }
    return FileHandlerManager.instance;
  }

  public addFileHandler(key: string, fileHandler: Handler) {
    this.fileHandlers.set(key, fileHandler);
  }

  public getFileHandler(key: string): Handler | undefined {
    return this.fileHandlers.get(key);
  }

  public removeFileHandler(key: string) {
    this.fileHandlers.delete(key);
  }
}
