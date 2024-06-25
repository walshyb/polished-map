#include <emscripten.h>
#include <stdio.h>
#include <iostream>
#include "filereader.h"
#include "../state.h"
#include <emscripten/bind.h>
#include "parse-png.h"
#include "../colors.h"

/**
 * Process a file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
bool FileProcessor::processFile(uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  std::string fn = std::string(filename);
  std::string ext = fn.substr(fn.find_last_of(".") + 1);

  // If ending in .ablk, process as ablk (map) file
  if (ext == "ablk") {
    processAblk(fileDataPtr, bufferSize, filename);
    return true;
  }

  if (ext == "bin") {
    std::cout << "Processing bin file" << std::endl;
    // if file_name contains "metatiles":
    if (fn.find("_metatiles") != std::string::npos) {
      return processMetatiles(fileDataPtr, bufferSize);
    }
  }

  // If ending in .pal, process as pal file, or palette
  if (ext == "pal") {
    return processPal(fileDataPtr, bufferSize, filename);
  }

  return false;
}

/**
 * Process a pal file. Creates a palette from the file
 * 
 * @param fileDataPtr Pointer to the file data
 * @param bufferSize Size of the file data
 * @param filename Point to the name of the file
 */
bool FileProcessor::processPal(const uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
  // Get (active) metatileset and tileset from state
  AppState *state = &AppState::getInstance();
  Metatileset *metatileset = state->getMetatileset();
  Tileset &tileset = metatileset->tileset();
  Palettes defaultPalettes = (Palettes)1;
  Palettes newPalettes = Color::read_palettes((char*)fileDataPtr, bufferSize, defaultPalettes);

  tileset.update_palettes(newPalettes);

  return true;
}

/**
 * Creates a metatileset from a .bin file
 */
bool FileProcessor::processMetatiles(const uint8_t* fileDataPtr, size_t bufferSize) {
  // Get (active) metatileset and tileset from state
  AppState *state = &AppState::getInstance();
  Metatileset *metatileset = state->getMetatileset();
  Tileset &tileset = metatileset->tileset();

  std::cout << "Process metatiles buffer size: " << bufferSize << std::endl;
  std::cout << "Metatileset Size before" << metatileset->size() << std::endl;

  Metatileset:: Result result = metatileset->read_metatiles(fileDataPtr, bufferSize);
  std::cout << "Metatileset Size after " << metatileset->size() << std::endl;
  std::cout << "Metatileset Result " << (bool)result << std::endl;

  return result == Metatileset::Result::META_OK;
}

extern "C" {
  /**
   * Read metatile data from .png and .2bpp files
   *
   * Accepts two tileset files (todo: and a roof file) and reads the graphics data.
   *
   * Tilesets can be named "tileset.beforeTileset.png" (e.g. "johto_traditional.johto_common.png").
   * In this case we'll need to read both files and concatenate the contents into a single tileset.
   *
   * If there is a tileset and an beforeTileset,
   * the two are concatenated into a single tileset. Otherwise, only 1 will be used.
   *
   * @param tilesetPtr Pointer to the base tileset data
   * @param tilesetBufferSize Size of the base tileset data
   * @param tilesetFilename full name of the tileset
   * @param beforeTilesetPtr Pointer to the before tileset data
   * @param beforeTilesetBufferSize Size of the before tileset data
   * @param beforeTilesetFilename name of the before tileset
   *
   * @return true if the files were read successfully, false otherwise
   *
   * TODO:
   * @param afterTilesetPtr Pointer to the after tileset data
   * @param afterTilesetBufferSize Size of the after tileset data
   * @param roofPtr Pointer to the roof data
   * @param roofBufferSize Size of the roof data
   * @param roofName Point to the name of the roof file
   */
  bool readMetatileData(
    uint8_t* tilesetPtr, size_t tilesetBufferSize, const char* tilesetFilename,
    uint8_t* beforeTilesetPtr, size_t beforeTilesetBufferSize, const char* beforeTilesetFilename
    //const uint8_t* roofPtr, size_t roofBufferSize, const char* roofName 
  ) {

    std::cout << "Read metatile data" << "Size: " << tilesetBufferSize << ", Filename: " << tilesetFilename << std::endl;
    std::cout << "Read metatile data" << "Size: " << beforeTilesetBufferSize << ", Beefore Filename: " << beforeTilesetFilename << std::endl;

    // Get (active) metatileset and tileset from state
    AppState *state = &AppState::getInstance();
    Metatileset *metatileset = state->getMetatileset();
    Tileset &tileset = metatileset->tileset();

    // Set tileset and roof tileset names
    tileset.name(tilesetFilename);
    //tileset.roof_name(roofName);
    
    // TODO: rename this struct
    // Make structs for libpng easy reading
    const PngData tilesetData = {
      .buf = tilesetPtr,
      .size = tilesetBufferSize,
      .pos = 0,
      .filename = tilesetFilename
    };

    const PngData beforeTilsetData = {
      .buf = beforeTilesetPtr,
      .size = beforeTilesetBufferSize,
      .pos = 0,
      .filename = beforeTilesetFilename
    };

    Tileset::Result rt = tileset.read_graphics(
      tilesetData,
      beforeTilsetData,
      tileset.palettes()
    );

    std::cout << "Read metatile data Result: " << (bool) rt << std::endl;

    return (bool) rt;
  }
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
      //std::cout << "Index: " << (int)i << ", val: " << static_cast<int>(id) << ", row: " << static_cast<int>(y) << ", col: " << static_cast<int>(x) << std::endl; // Print ID
    }
  }
  std::cout << "File processed" << std::endl; // Print ID
}

// Export the function to JavaScript
extern "C" {
  FileProcessor fileProcessor;

  bool processFile(uint8_t* fileDataPtr, size_t bufferSize, const char* filename) {
    return fileProcessor.processFile(fileDataPtr, bufferSize, filename);
  }
}
