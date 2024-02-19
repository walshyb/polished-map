#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"

AppState::~AppState() {
  map->clear();
}

AppState::AppState() {
  map = new Map();
}

Map* AppState::getMap() {
  return map;
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  Block ** getBlocks() {
    AppState *state = &AppState::getInstance();
    Map *map = state->getMap();
    std::vector<Block> blocksResult;
    std::cout << "Block size: " << sizeof(map->_blocks[0])  << std::endl;
    std::cout << "Blocks size: " << sizeof(map->_blocks)  << std::endl;
    std::cout << "Blocks length: " << map->_numBlocks  << std::endl;
    Block ** blocks = map->_blocks;
    return blocks;
  }
}
