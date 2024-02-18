#include <cstdio>

#include "map.h"

void Map_Attributes::clear() {
	group = 0;
}

Map::Map() : _attributes(), _width(0), _height(0), _blocks(NULL), _result(Result::MAP_NULL), _modified(false), _mod_time(0),
	_history(), _future() {}

void Map::clear() {
  delete [] _blocks;
  _blocks = NULL;
  _width = _height = 0;
  _result = Result::MAP_NULL;
  _modified = false;
  _mod_time = 0;
  _history.clear();
  _future.clear();
}

Map::~Map() {
	clear();
}


