#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"

AppState::~AppState() {
  map->clear();
}

AppState::AppState() {
  map = new Map();
  metaTileSet = new Metatileset();
}

Map* AppState::getMap() {
  return map;
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  Block ** getBlocks() {
    AppState *state = &AppState::getInstance();
    Map *map = state->getMap();
    Block ** blocks = map->_blocks;
    return blocks;
  }
}
