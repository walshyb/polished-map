#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"

AppState::~AppState() {
  map.clear();
}

Map AppState::getMap() {
  return map;
}
