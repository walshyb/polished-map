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

