#ifndef TILESET_H
#define TILESET_H

#include <string>

#include "deep-tile.h"
#include "tiled-image.h"
#include "../../utils.h"
#include "../../utils/parse-png.h"

#define MAX_NUM_TILES 512

#define TILES_PER_ROW 16
#define TILES_PER_COL (MAX_NUM_TILES / TILES_PER_ROW)

#define FIRST_ROOF_TILE_IDX 0x00a
#define NUM_ROOF_TILES 9
#define ROOF_TILES_PER_ROW 3
#define ROOF_TILES_PER_COL (NUM_ROOF_TILES / ROOF_TILES_PER_ROW)

class Tileset {
public:
	enum class Result { GFX_OK, GFX_BAD_FILE, GFX_BAD_EXT, GFX_BAD_DIMS, GFX_TOO_SHORT,
		GFX_TOO_LARGE, GFX_NOT_GRAYSCALE, GFX_BAD_CMD, GFX_NULL };
private:
	std::string _name, _roof_name;
	Palettes _palettes;
	Deep_Tile *_tiles[MAX_NUM_TILES], *_roof_tiles[MAX_NUM_TILES];
	size_t _num_tiles, _num_before_tiles, _num_mid_tiles, _num_roof_tiles;
	Result _result;
	bool _modified, _modified_roof;
	int64_t _mod_time, _mod_time_before, _mod_time_after, _mod_time_roof;
public:
	Tileset();
	~Tileset();
	inline const char *name(void) const { return _name.c_str(); }
	inline void name(const char *m) { _name = m ? m : ""; }
	inline const char *roof_name(void) const { return _roof_name.c_str(); }
	inline void roof_name(const char *m) { _roof_name = m ? m : ""; }
	inline bool has_roof(void) const { return !_roof_name.empty(); }
	inline Palettes palettes(void) const { return _palettes; }
	inline Deep_Tile *tile(int i) { return _tiles[i]; }
	inline Deep_Tile *roof_tile(int i) { return _roof_tiles[i]; }
	inline const Deep_Tile *const_tile(int i) const { return _tiles[i]; }
	inline const Deep_Tile *const_roof_tile(int i) const { return _roof_tiles[i]; }
	inline const Deep_Tile *const_tile_or_roof(int i) const {
		return (i >= FIRST_ROOF_TILE_IDX && (size_t)i < FIRST_ROOF_TILE_IDX + _num_roof_tiles) ? _roof_tiles[i] : _tiles[i];
	}
	inline size_t num_tiles(void) const { return _num_tiles; }
	inline size_t num_roof_tiles(void) const { return _num_roof_tiles; }
	inline Result result(void) const { return _result; }
	inline bool modified(void) const { return _modified; }
	inline void modified(bool m) { _modified = m; }
	inline bool modified_roof(void) const { return _modified_roof; }
	inline void modified_roof(bool m) { _modified_roof = m; }
	inline bool other_modified(const char *f) const { return file_modified(f) > _mod_time; }
	inline bool other_modified_before(const char *bf) const { return file_modified(bf) > _mod_time_before; }
	inline bool other_modified_after(const char *af) const { return file_modified(af) > _mod_time_after; }
	inline bool other_modified_roof(const char *rf) const { return file_modified(rf) > _mod_time_roof; }
private:
	void read_tile(Deep_Tile *dt, const Tiled_Image &ti, size_t i);
	static void print_tile_rgb(const Deep_Tile *dt, int tx, int ty, int n, unsigned char *buffer);
	Result convert_tiled_image_result(Tiled_Image::Result r);
public:
	void clear(void);
	void clear_roof_graphics(void);
	void update_palettes(Palettes l);
	unsigned char *print_rgb(size_t w, size_t h, size_t off, size_t n) const;
	unsigned char *print_roof_rgb(size_t w, size_t h) const;
  Result read_graphics(const PngData tileset, const PngData beforeTilesetPtr, Palettes l);
	Result read_roof_graphics(const char *f);
	static const char *error_message(Result result);
	const char *write_graphics(const char *f, const char *bf, const char *af);
	bool write_roof_graphics(const char *f);
};

#endif
