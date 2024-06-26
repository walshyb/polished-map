#include "map/map.h"
#include <emscripten.h>
#include "state.h"

AppState::~AppState() {
  map->clear();
  metatileset->clear();
}

AppState::AppState() {
  map = new Map();
  metatileset = new Metatileset();
}

Map* AppState::getMap() {
  return map;
}

Metatileset* AppState::getMetatileset() {
  return metatileset;
}

void AppState::destroy() {
  delete this;
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
