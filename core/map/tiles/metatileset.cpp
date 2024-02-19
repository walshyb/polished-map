#include <cstdio>
#include <fstream>
#include <sstream>

#include "metatileset.h"
#include "../../parse-asm.h"

Metatileset::Metatileset() : _tileset(), _metatiles(), _num_metatiles(0), _result(Result::META_NULL), _modified(false),
	_bin_collisions(false), _mod_time(0), _mod_time_coll(0) {
	for (size_t i = 0; i < MAX_NUM_METATILES; i++) {
		_metatiles[i] = new Metatile((uint8_t)i);
	}
}

Metatileset::~Metatileset() {
	clear();
	for (Metatile *mt : _metatiles) {
		delete mt;
	}
}

void Metatileset::clear() {
	_tileset.clear();
	for (Metatile *mt : _metatiles) {
		mt->clear();
	}
	_num_metatiles = 0;
	_result = Result::META_NULL;
	_modified = false;
	_bin_collisions = false;
	_mod_time = _mod_time_coll = 0;
}

void Metatileset::size(size_t n) {
	size_t low = std::min(n, _num_metatiles), high = std::max(n, _num_metatiles);
	for (size_t i = low; i < high; i++) {
		_metatiles[i]->clear();
	}
	_num_metatiles = n;
	_modified = true;
}

bool Metatileset::uses_tile(uint8_t id) const {
	for (size_t i = 0; i < _num_metatiles; i++) {
		if (_metatiles[i]->uses_tile(id)) {
			return true;
		}
	}
	return false;
}

void Metatileset::trim_tileset() {
	for (uint8_t i = MAX_NUM_TILES - 1; i > 0; i--) {
		Tile *t = _tileset.tile(i);
		if (t->palette() != Palette::UNDEFINED && (!t->is_blank() || uses_tile(i))) {
			break;
		}
		t->palette(Palette::UNDEFINED);
		_tileset.palette_map().palette(i, Palette::UNDEFINED);
	}
}

void Metatileset::draw_metatile(int x, int y, uint8_t id, bool zoom, bool show_priority) const {
	if (id < size()) {
		Metatile *mt = _metatiles[id];
		int s = TILE_SIZE * (zoom ? ZOOM_FACTOR : 1);
		for (int ty = 0; ty < METATILE_SIZE; ty++) {
			int ay = y + ty * s;
			for (int tx = 0; tx < METATILE_SIZE; tx++) {
				int ax = x + tx * s;
				uint8_t tid = mt->tile_id(tx, ty);
				const Tile *t = _tileset.const_tile_or_roof(tid);
				t->draw_with_priority(ax, ay, zoom ? TILE_PX_SIZE : TILE_SIZE, show_priority);
			}
		}
	}
	else {
		int s = TILE_SIZE * METATILE_SIZE * (zoom ? ZOOM_FACTOR : 1);
	}
}

const char *Metatileset::error_message(Result result) {
	switch (result) {
	case Result::META_OK:
		return "OK.";
	case Result::META_NO_GFX:
		return "No corresponding graphics file chosen.";
	case Result::META_BAD_FILE:
		return "Cannot open file.";
	case Result::META_TOO_SHORT:
		return "The last block is incomplete.";
	case Result::META_TOO_LONG:
		return "More than 256 blocks defined.";
	case Result::META_NULL:
		return "No blockset file chosen.";
	default:
		return "Unspecified error.";
	}
}

