#include <emscripten.h>
#include <stdio.h>
#include <iostream>


extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    // Process file data here
    // Example: Printing the first 10 characters
    std::cout << "File: " << filename << std::endl; 
    std::cout << "File Size: " << bufferSize << std::endl; 

    for(size_t i = 0; i < bufferSize; i++) {
      // Access buffer element
      uint8_t value = *(fileDataPtr + i);
      std::cout << value << std::endl;
    }
  }
}
