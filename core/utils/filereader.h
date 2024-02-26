#include "../state.h"
#define 	FL_PATH_MAX   2048

class FileProcessor {
public:
  bool processFile(uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  void processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool processPal(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool processMetatiles(const uint8_t* fileDataPtr, size_t bufferSize);
  bool readMetatileData(
    uint8_t* tilesetPtr, size_t tilesetBufferSize, const char* tilesetFilename,
    uint8_t* beforeTilesetPtr, size_t beforeTilesetBufferSize, const char* beforeTilesetFilename,
    const uint8_t* roofPtr, size_t roofBufferSize, const char* roofName 
  );
};
