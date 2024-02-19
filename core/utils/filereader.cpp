#include <emscripten.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include "filereader.h"

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    switch(.substr(fn.find_last_of(".") + 1)) {
      case 'albk':
        processAblk(fileDataPtr, bufferSize, filename);
        break;
      default:
        break;
    }
  }

  void processAlbk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    // Process file data here
    // Example: Printing the first 10 characters
    std::cout << "File: " << filename << std::endl; 
    std::cout << "File Size: " << bufferSize << std::endl; 
    int _width = 28;
    int _height = 18;
    // open violetCity

    for (uint8_t y = 0; y < (size_t)_height; y++) {
      for (uint8_t x = 0; x < (size_t)_width; x++) {
        size_t i = (size_t)y * _width + (size_t)x;
        uint8_t id = fileDataPtr[i];
        std::cout << "Index: " << i << ", val: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(id) << std::endl;
        // _blocks[i] = new Block(y, x, id);
      }
    }

    for(size_t i = 0; i < bufferSize; i++) {
      // Access buffer element
     // uint8_t value = *(fileDataPtr + i);
      //std::cout << value << std::endl;
    }
  }
}
