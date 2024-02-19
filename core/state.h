#ifndef APPSTATE
#define APPSTATE

#include "map/map.h"
class AppState {

public:
  static AppState& getInstance() {
    static AppState instance;
    return instance;
  }

  Map *getMap();
private:
  AppState();
  ~AppState();
  Map *map;
};

#endif
