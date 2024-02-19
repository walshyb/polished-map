#include "../state.h"

class FileProcessor {
public:
  void processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  void processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
};
