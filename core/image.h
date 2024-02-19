#ifndef IMAGE_H
#define IMAGE_H

#include "map/map.h"
#include "map/tiles/metatileset.h"
#include "map/tiles/tileset.h"

class Image {
public:
	enum class Result { IMAGE_OK, IMAGE_BAD_DATA, IMAGE_BAD_FILE, IMAGE_BAD_PNG, IMAGE_EMPTY };
	static Result write_tileset_image(const char *f, const Tileset &tileset, size_t off = 0x00, size_t n = 0);
	static Result write_roof_image(const char *f, const Tileset &tileset);
	static const char *error_message(Result result);
private:
	static Result write_image(const char *f, size_t w, size_t h, unsigned char *buffer, bool is_2bpp = false,
		int pd = 1, int d = 3, int ld = 0);
};

#endif

