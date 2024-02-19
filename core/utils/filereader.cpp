#include <emscripten.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include "filereader.h"
#include "../state.h"
#include <emscripten/bind.h>
#include "../map/block.h"

/**
 * Process a file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
void FileProcessor::processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  std::string fn = std::string(filename);

  // If ending in .ablk, process as ablk
  if (fn.substr(fn.find_last_of(".") + 1) == "ablk") {
    processAblk(fileDataPtr, bufferSize, filename);
  }
}

/**
 * Process an ablk file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
// TODO: maybe have this call Map::read_blocks
void FileProcessor::processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  int _width = 28;
  int _height = 18;

  AppState *state = &AppState::getInstance();
  Map *map = state->getMap();
  map->size(_width, _height);

  // Loop through input
  // Create block for each tile
  for (uint8_t y = 0; y < (size_t)_height; y++) {
    for (uint8_t x = 0; x < (size_t)_width; x++) {
      size_t i = (size_t)y * _width + (size_t)x;
      uint8_t id = fileDataPtr[i];
        map->addBlock(i, new Block(y, x, id));


      // std::cout << "Index: " << i << ", val: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(id) << std::endl; // Print ID
    }
  }
}

// Export the function to JavaScript
extern "C" {
  FileProcessor fileProcessor;

  void processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    fileProcessor.processFile(fileDataPtr, bufferSize, filename);
  }
}
