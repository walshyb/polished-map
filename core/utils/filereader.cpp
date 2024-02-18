#include <emscripten.h>
#include <stdio.h>

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void processFile(const char* fileData, size_t size) {
    // Process file data here
    // Example: Printing the first 10 characters
    for (int i = 0; i < 10; ++i) {
      printf("%c", fileData[i]);
    }
  }
}
