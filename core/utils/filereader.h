#include "../state.h"
#define 	FL_PATH_MAX   2048

class FileProcessor {
public:
  bool processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  void processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
  bool processPng(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename);
};
