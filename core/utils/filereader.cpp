#include <emscripten.h>
#include <stdio.h>
#include <iostream>


extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void processFile(const char* fileData, size_t size, const char* filename) {
    // Process file data here
    // Example: Printing the first 10 characters
    std::cout << "File: " << filename << std::endl; 
    std::cout << "Size: " << size << std::endl; 
  }
}
