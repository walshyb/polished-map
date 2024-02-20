#ifndef CHIP_H
#define CHIP_H

#include "tile.h"

class Chip {
private:
	uint8_t _row, _col;
	int _index;
	Palette _palette;
	bool _x_flip, _y_flip, _priority;
public:
	Chip(int x, int y, int s, uint8_t row, uint8_t col);
	inline uint8_t row(void) const { return _row; }
	inline uint8_t col(void) const { return _col; }
	inline void coords(uint8_t row, uint8_t col) { _row = row; _col = col; }
	inline int index(void) const { return _index; }
	inline void index(int idx) { _index = idx; }
	inline Palette palette(void) const { return _palette; }
	inline void palette(Palette p) { _palette = p; }
	inline bool x_flip(void) const { return _x_flip; }
	inline void x_flip(bool f) { _x_flip = f; }
	inline bool y_flip(void) const { return _y_flip; }
	inline void y_flip(bool f) { _y_flip = f; }
	inline bool priority(void) const { return _priority; }
	inline void priority(bool p) { _priority = p; }
	//void copy(const Tile *t);
	//void copy(const Tile_Button *tb);
	void draw(void);
	int handle(int event);
};

#endif
