#include "map/map.h"
class AppState {
public:
  AppState() : map() {};
  ~AppState();
  Map getMap();
private:
  Map map;
};
