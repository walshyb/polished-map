#ifndef APPSTATE
#define APPSTATE

#include "map/map.h"
#include "map/tiles/metatileset.h"

class AppState {

public:
  static AppState& getInstance() {
    static AppState instance;
    return instance;
  }

  Map *getMap();
  Metatileset *getMetatileset();
private:
  AppState();
  ~AppState();
  Map *map;
  Metatileset *metatileset;
};

#endif
