#include <emscripten.h>
#include <stdio.h>
#include <iostream>
#include "filereader.h"
#include "../state.h"
#include <emscripten/bind.h>
#include "../map/block.h"
#include "../map/map.h"
#include "parse-png.h"

/**
 * Process a file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
bool FileProcessor::processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  std::string fn = std::string(filename);
  std::string ext = fn.substr(fn.find_last_of(".") + 1);

  // If ending in .ablk, process as ablk
  if (ext == "ablk") {
    processAblk(fileDataPtr, bufferSize, filename);
    return true;
  }

  if (ext == "png") {
    return processPng(fileDataPtr, bufferSize, filename);
  }

  return false;
}

/**
 * Accepts two tileset files and a roof file and reads the graphics data.
 *
 * Tilesets can be named "bforeTileset.afterTileset.png" (e.g. "johto_traditional.johto_common.png").
 * In this case we'll need to read both files and concatenate them into a single tileset.
 *
 * If there is a beforeTileset and an afterTileset,
 * the two are concatenated into a single tileset. Otherwise, only 1 will be used.
 *
 * @param fullTilesetName Name of the full tileset
 * @param fullTilesetSize Size of the full tileset data
 * @param beforeTilesetPtr Pointer to the before tileset data
 * @param beforeTilesetBufferSize Size of the before tileset data
 * @param beforeTilesetFilename Point to the name of the before tileset file
 * @param afterTilesetPtr Pointer to the after tileset data
 * @param afterTilesetBufferSize Size of the after tileset data
 * @param afterTilesetFilename Point to the name of the after tileset file
 * @param roofPtr Pointer to the roof data
 * @param roofBufferSize Size of the roof data
 * @param roofName Point to the name of the roof file
 * @return true if the file was read successfully, false otherwise
 */
bool FileProcessor::readMetatileData(
    const char* fullTilesetName, size_t fullTilesetSize,
    const uint8_t* beforeTilesetPtr, size_t beforeTilesetBufferSize, const char* beforeTilesetFilename,
    const uint8_t* afterTilesetPtr, size_t afterTilesetBufferSize, const char* afterTilesetFilename,
    const uint8_t* roofPtr, size_t roofBufferSize, const char* roofName 
  ) {

  // Get (active) metatileset and tileset from state
  AppState *state = &AppState::getInstance();
  Metatileset *metatileset = state->getMetatileset();
	Tileset &tileset = metatileset->tileset();

  // Set tileset and roof tileset names
  tileset.name(fullTilesetName);
  tileset.roof_name(roofName);

  // Hold contents of before &| after tilesets
  char buffer[FL_PATH_MAX] = {};

  // Make structs for libpng easy reading
  PngData beforeTilsetPng = {
    .buf = beforeTilesetPtr,
    .size = beforeTilesetBufferSize,
    .pos = 0
  };

  PngData afterTilsetPng = {
    .buf = afterTilesetPtr,
    .size = afterTilesetBufferSize,
    .pos = 0
  };

  Tileset::Result rt = tileset.read_graphics(
    buffer,
    beforeTilesetBufferSize ? &beforeTilsetPng : NULL,
    afterTilesetBufferSize ? &afterTilsetPng : NULL,
    // todo add current palettes
    palettes 
    );


  //return metatileset->readMetatileData(tilesetPtr, bufferSize, filename);
}

/**
 * Process a png file. Gets the width, height, and depth of the image
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
bool FileProcessor::processPng(const uint8_t* bufferPtr, size_t bufferSize, const char* filename) {
  int width, height, depth;

  bool result = getPngData(bufferPtr, bufferSize, width, height, depth);

  if (result) {
    std::cout << "Width: " << width << ", Height: " << height << ", Depth: " << depth << std::endl;
  }

  return result;
}

/**
 * Process an ablk file. Creates blocks for each ID in the file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
// TODO: maybe have this call Map::read_blocks
// TODO: send OK flag
void FileProcessor::processAblk(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  int _width = 20;
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


      std::cout << "Index: " << (int)i << ", val: " << static_cast<int>(id) << ", row: " << static_cast<int>(y) << ", col: " << static_cast<int>(x) << std::endl; // Print ID
    }
  }
  std::cout << "File processed" << std::endl; // Print ID
}

// Export the function to JavaScript
extern "C" {
  FileProcessor fileProcessor;

  bool processFile(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    return fileProcessor.processFile(fileDataPtr, bufferSize, filename);
  }
}
