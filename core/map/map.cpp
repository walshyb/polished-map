#include <cstdio>

#include "map.h"

void Map_Attributes::clear() {
	group = 0;
}

Map::Map() : _attributes(), _width(0), _height(0), _blocks(NULL), _result(Result::MAP_NULL), _modified(false), _mod_time(0),
	_history(), _future() {}

void Map::clear() {
  delete [] _blocks;
  _blocks = NULL;
  _width = _height = 0;
  _result = Result::MAP_NULL;
  _modified = false;
  _mod_time = 0;
  _history.clear();
  _future.clear();
}

Map::~Map() {
	clear();
}


/**
 * @brief Read the blocks from a file
 */
/*
Map::Result Map::read_blocks(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
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
}*/

void Map::addBlock(uint8_t index, Block * block) {
  _blocks[index] = block;
}

void Map::size(uint8_t width, uint8_t height) {
  _width = width;
  _height = height;

  // Create space for new blocks
  _blocks = new Block *[size()]();
}
