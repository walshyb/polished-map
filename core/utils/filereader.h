#include "../state.h"
#define 	FL_PATH_MAX   2048

class FileProcessor {
public:
  bool processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  void processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool processPng(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool processPal(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool readMetatileData(
    const char* fullTilesetName, size_t fullTilesetSize,
    const uint8_t* beforeTilesetPtr, size_t beforeTilesetBufferSize, const char* beforeTilesetFilename,
    const uint8_t* afterTilesetPtr, size_t afterTilesetBufferSize, const char* afterTilesetFilename,
    const uint8_t* roofPtr, size_t roofBufferSize, const char* roofName 
  );
};
