#include "../../config.h"
#include "tileset.h"
#include "../../image.h"

Tileset::Tileset() : _name(), _palettes(), _tiles(), _roof_tiles(), _num_tiles(0), _num_before_tiles(0),
	_num_mid_tiles(0), _num_roof_tiles(0), _result(Result::GFX_NULL), _modified(false), _modified_roof(false),
	_mod_time(0), _mod_time_before(0), _mod_time_after(0), _mod_time_roof(0) {
	for (size_t i = 0; i < MAX_NUM_TILES; i++) {
		_tiles[i] = new Deep_Tile((int)i);
		_roof_tiles[i] = new Deep_Tile((int)i);
	}
}

Tileset::~Tileset() {
	clear();
	for (size_t i = 0; i < MAX_NUM_TILES; i++) {
		delete _tiles[i];
		delete _roof_tiles[i];
	}
}

void Tileset::clear() {
	_name.clear();
	_num_tiles = 0;
	_num_before_tiles = 0;
	_num_mid_tiles = 0;
	for (Deep_Tile *dt : _tiles) {
		dt->clear();
	}
	clear_roof_graphics();
	_result = Result::GFX_NULL;
	_modified = false;
	_modified_roof = false;
	_mod_time = _mod_time_before = _mod_time_after = _mod_time_roof = 0;
}

void Tileset::clear_roof_graphics() {
	_num_roof_tiles = 0;
	for (Deep_Tile *dt : _roof_tiles) {
		dt->clear();
	}
}

void Tileset::update_palettes(Palettes l) {
	_palettes = l;
	for (int i = 0; i < MAX_NUM_TILES; i++) {
		_tiles[i]->update_palettes(l);
		_roof_tiles[i]->update_palettes(l);
	}
}

unsigned char *Tileset::print_rgb(size_t w, size_t h, size_t off, size_t n) const {
	unsigned char *buffer = new unsigned char[w * h * NUM_CHANNELS]();
	std::fill_n(buffer, w * h * NUM_CHANNELS, (unsigned char)0xff);
	for (size_t i = 0; i < n; i++) {
		size_t j = i + off;
		const Deep_Tile *dt = _tiles[j];
		int ty = (i / TILES_PER_ROW) * TILE_SIZE, tx = (i % TILES_PER_ROW) * TILE_SIZE;
		print_tile_rgb(dt, tx, ty, TILES_PER_ROW, buffer);
	}
	return buffer;
}

unsigned char *Tileset::print_roof_rgb(size_t w, size_t h) const {
	unsigned char *buffer = new unsigned char[w * h * NUM_CHANNELS]();
	std::fill_n(buffer, w * h * NUM_CHANNELS, (unsigned char)0xff);
	for (size_t i = 0; i < NUM_ROOF_TILES; i++) {
		const Deep_Tile *dt = _roof_tiles[i + FIRST_ROOF_TILE_IDX];
		int ty = (i / ROOF_TILES_PER_ROW) * TILE_SIZE, tx = (i % ROOF_TILES_PER_ROW) * TILE_SIZE;
		print_tile_rgb(dt, tx, ty, ROOF_TILES_PER_ROW, buffer);
	}
	return buffer;
}

void Tileset::read_tile(Deep_Tile *dt, const Tiled_Image &ti, size_t i) {
	dt->undefined(false);
	for (int ty = 0; ty < TILE_SIZE; ty++) {
		for (int tx = 0; tx < TILE_SIZE; tx++) {
			Hue h = ti.tile_hue(i, tx, ty);
			dt->render_pixel(tx, ty, _palettes, h);
		}
	}
}

void Tileset::print_tile_rgb(const Deep_Tile *dt, int tx, int ty, int n, unsigned char *buffer) {
	for (int py = 0; py < TILE_SIZE; py++) {
		for (int px = 0; px < TILE_SIZE; px++) {
			Hue h = dt->hue(px, py);
			const unsigned char *rgb = Color::monochrome_color(h);
			size_t j = ((ty + py) * n * TILE_SIZE + tx + px) * NUM_CHANNELS;
			buffer[j++] = rgb[0];
			buffer[j++] = rgb[1];
			buffer[j]   = rgb[2];
		}
	}
}

Tileset::Result Tileset::convert_tiled_image_result(Tiled_Image::Result r) {
	switch (r) {
	case Tiled_Image::Result::IMG_OK: return Result::GFX_OK;
	case Tiled_Image::Result::IMG_NULL: return (_result = Result::GFX_BAD_FILE);
	case Tiled_Image::Result::IMG_BAD_FILE: return (_result = Result::GFX_BAD_FILE);
	case Tiled_Image::Result::IMG_BAD_EXT: return (_result = Result::GFX_BAD_EXT);
	case Tiled_Image::Result::IMG_BAD_DIMS: return (_result = Result::GFX_BAD_DIMS);
	case Tiled_Image::Result::IMG_TOO_SHORT: return (_result = Result::GFX_TOO_SHORT);
	case Tiled_Image::Result::IMG_TOO_LARGE: return (_result = Result::GFX_TOO_LARGE);
	case Tiled_Image::Result::IMG_NOT_GRAYSCALE: return (_result = Result::GFX_NOT_GRAYSCALE);
	case Tiled_Image::Result::IMG_BAD_CMD: return (_result = Result::GFX_BAD_CMD);
	default: return (_result = Result::GFX_BAD_FILE);
	}
}


/**
 * Build tiles off of passed in .png tilesets
 *
 * @param tileset The tileset png data. i.e. johto_overcast.johto_common.png
 * @param beforeTileset The before tileset png data. i.e. johto_common.png
 * @param l The palettes
 *
 * @return The result of the operation
 */
Tileset::Result Tileset::read_graphics(
    const PngData tileset,
    const PngData beforeTileset,
    Palettes l
) {
	Tiled_Image bti(beforeTileset);
	size_t bn = bti.num_tiles();
	if (beforeTileset.size && convert_tiled_image_result(bti.result()) != Result::GFX_OK) { return _result; }

	Tiled_Image ti(tileset);
	size_t mn = ti.num_tiles();
	if (convert_tiled_image_result(ti.result()) != Result::GFX_OK) { return _result; }

	_num_before_tiles = ((bn + TILES_PER_ROW - 1) / TILES_PER_ROW) * TILES_PER_ROW;
  // TODO implement after tile image one day
  //Tiled_Image ati(afterTileset);
	//size_t an = ati.num_tiles();
	//if (afterTileset.size && convert_tiled_image_result(ati.result()) != Result::GFX_OK) { return _result; }
	//_num_mid_tiles = an ? ((mn + TILES_PER_ROW - 1) / TILES_PER_ROW) * TILES_PER_ROW : mn;
	//_num_tiles = _num_before_tiles + (an ? _num_mid_tiles + an : mn);

	_num_mid_tiles = mn;
	_num_tiles = _num_before_tiles + mn;
	if (_num_tiles > 0x100) {
		Config::allow_512_tiles(true);
	}

	_palettes = l;
	for (size_t i = 0; i < _num_tiles; i++) {
		Deep_Tile *dt = _tiles[i];
		if ((size_t)i < bn) {
			read_tile(dt, bti, i);
		}
		else if ((size_t)i >= _num_before_tiles && (size_t)i < _num_before_tiles + mn) {
			read_tile(dt, ti, i - _num_before_tiles);
		}
		else if ((size_t)i >= _num_before_tiles + _num_mid_tiles) {
      std::cerr << "Made it to after tiles land" << std::endl;
      // TODO implement after tile image one day
			//read_tile(dt, ati, i - _num_before_tiles - _num_mid_tiles);
		}
	}

	_modified = false;
	_modified_roof = false;
	//_mod_time = file_modified(f);
	//_mod_time_before = file_modified(bf);
	//_mod_time_after = file_modified(af);

	return (_result = Result::GFX_OK);
}

Tileset::Result Tileset::read_roof_graphics(const char *f) {
	Tiled_Image ti(f);
	switch (ti.result()) {
	case Tiled_Image::Result::IMG_OK: break;
	case Tiled_Image::Result::IMG_NULL: return Result::GFX_BAD_FILE;
	case Tiled_Image::Result::IMG_BAD_FILE: return Result::GFX_BAD_FILE;
	case Tiled_Image::Result::IMG_BAD_EXT: return Result::GFX_BAD_EXT;
	case Tiled_Image::Result::IMG_BAD_DIMS: return Result::GFX_BAD_DIMS;
	case Tiled_Image::Result::IMG_TOO_SHORT: return Result::GFX_TOO_SHORT;
	case Tiled_Image::Result::IMG_TOO_LARGE: return Result::GFX_TOO_LARGE;
	case Tiled_Image::Result::IMG_NOT_GRAYSCALE: return Result::GFX_NOT_GRAYSCALE;
	case Tiled_Image::Result::IMG_BAD_CMD: return Result::GFX_BAD_CMD;
	default: return Result::GFX_BAD_FILE;
	}

	if (ti.num_tiles() < NUM_ROOF_TILES) {
		return Result::GFX_TOO_SHORT;
	}
	if (ti.num_tiles() > NUM_ROOF_TILES || FIRST_ROOF_TILE_IDX + ti.num_tiles() > MAX_NUM_TILES) {
		return Result::GFX_TOO_LARGE;
	}
	_num_roof_tiles = ti.num_tiles();

	for (size_t i = 0; i < _num_roof_tiles; i++) {
		int k = (int)i + FIRST_ROOF_TILE_IDX;
		Deep_Tile *dt = _roof_tiles[k];
		read_tile(dt, ti, i);
	}

	_mod_time_roof = file_modified(f);

	return Result::GFX_OK;
}

const char *Tileset::error_message(Result result) {
	switch (result) {
	case Result::GFX_OK:
		return "OK.";
	case Result::GFX_BAD_FILE:
		return "Cannot parse file format.";
	case Result::GFX_BAD_EXT:
		return "Unknown file extension.";
	case Result::GFX_BAD_DIMS:
		return "Image dimensions do not fit the tile grid.";
	case Result::GFX_TOO_SHORT:
		return "Too few bytes.";
	case Result::GFX_TOO_LARGE:
		return "Too many pixels.";
	case Result::GFX_NOT_GRAYSCALE:
		return "Image cannot be made grayscale.";
	case Result::GFX_BAD_CMD:
		return "Invalid LZ command.";
	case Result::GFX_NULL:
		return "No graphics file chosen.";
	default:
		return "Unspecified error.";
	}
}

const char *Tileset::write_graphics(const char *f, const char *bf, const char *af) {
	if (bf[0]) {
		Image::Result b_result = Image::write_tileset_image(bf, *this, 0, _num_before_tiles);
		if (b_result != Image::Result::IMAGE_OK) { return bf; }
		_mod_time_before = file_modified(bf);
	}
	if (af[0]) {
		Image::Result result = Image::write_tileset_image(f, *this, _num_before_tiles, _num_mid_tiles);
		if (result != Image::Result::IMAGE_OK) { return f; }
		_mod_time = file_modified(f);
		Image::Result a_result = Image::write_tileset_image(af, *this, _num_before_tiles + _num_mid_tiles, 0);
		if (a_result != Image::Result::IMAGE_OK) { return af; }
		_mod_time_after = file_modified(af);
		return NULL;
	}
	else {
		Image::Result result = Image::write_tileset_image(f, *this, _num_before_tiles, 0);
		if (result != Image::Result::IMAGE_OK) { return f; }
		_mod_time = file_modified(f);
		return NULL;
	}
}

bool Tileset::write_roof_graphics(const char *f) {
	Image::Result result = Image::write_roof_image(f, *this);
	if (result != Image::Result::IMAGE_OK) { return false; }
	_mod_time_roof = file_modified(f);
	return true;
}
