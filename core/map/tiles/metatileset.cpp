#include <cstdio>
#include "metatileset.h"
#include "../../utils/parse-png.h"
#include "../../colors.h"

// 8x8 translucent zigzag pattern for tile priority
static unsigned char small_priority_png_buffer[] = {
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x02, 0x03, 0x00, 0x00, 0x00, 0xb9, 0x61, 0x56,
	0x18, 0x00, 0x00, 0x00, 0x0c, 0x50, 0x4c, 0x54, 0x45, 0x68, 0xd8, 0xd8, 0x56, 0xba, 0xba, 0x32,
	0x7e, 0x7e, 0x20, 0x60, 0x60, 0xb4, 0x24, 0x6a, 0xe6, 0x00, 0x00, 0x00, 0x04, 0x74, 0x52, 0x4e,
	0x53, 0x60, 0x60, 0x60, 0x60, 0xe8, 0x2d, 0x50, 0x46, 0x00, 0x00, 0x00, 0x16, 0x49, 0x44, 0x41,
	0x54, 0x78, 0x5e, 0x63, 0x90, 0x90, 0x60, 0xa8, 0xab, 0x63, 0x78, 0xfe, 0x9c, 0xa1, 0xb1, 0x11,
	0x99, 0x0d, 0x00, 0x55, 0xe0, 0x07, 0xf9, 0x01, 0x25, 0x72, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x49,
	0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

//static Fl_PNG_Image small_priority_png(NULL, small_priority_png_buffer, sizeof(small_priority_png_buffer));

// 16x16 translucent zigzag pattern for tile priority
static unsigned char large_priority_png_buffer[] = {
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
	0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x02, 0x03, 0x00, 0x00, 0x00, 0x62, 0x9d, 0x17,
	0xf2, 0x00, 0x00, 0x00, 0x0c, 0x50, 0x4c, 0x54, 0x45, 0x20, 0x60, 0x60, 0x32, 0x7e, 0x7e, 0x56,
	0xba, 0xba, 0x68, 0xd8, 0xd8, 0xdc, 0xe3, 0x64, 0x6e, 0x00, 0x00, 0x00, 0x04, 0x74, 0x52, 0x4e,
	0x53, 0x60, 0x60, 0x60, 0x60, 0xe8, 0x2d, 0x50, 0x46, 0x00, 0x00, 0x00, 0x2e, 0x49, 0x44, 0x41,
	0x54, 0x78, 0x5e, 0x63, 0xf8, 0x57, 0xff, 0xaf, 0x9e, 0xe1, 0x87, 0xfc, 0x0f, 0x79, 0x86, 0x07,
	0xec, 0x0f, 0xd8, 0x19, 0x1a, 0x18, 0x1b, 0x18, 0x19, 0x80, 0xb8, 0x81, 0x01, 0xc8, 0x7b, 0xc0,
	0x20, 0x0f, 0x94, 0x61, 0x00, 0xaa, 0xf8, 0x47, 0xac, 0x3a, 0x00, 0xfb, 0x8c, 0x1f, 0xe1, 0x3f,
	0x4c, 0xa1, 0xea, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

//static Fl_PNG_Image large_priority_png(NULL, large_priority_png_buffer, sizeof(large_priority_png_buffer));

Metatileset::Metatileset() : _tileset(), _metatiles(), _num_metatiles(0), _result(Result::META_NULL), _modified(false),
	_bin_collisions(false), _mod_time(0), _mod_time_attr(0), _mod_time_coll(0) {
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
	_mod_time = _mod_time_attr = _mod_time_coll = 0;
}

void Metatileset::size(size_t n) {
	size_t low = std::min(n, _num_metatiles), high = std::max(n, _num_metatiles);
	for (size_t i = low; i < high; i++) {
		_metatiles[i]->clear();
	}
	_num_metatiles = n;
	_modified = true;
}

bool Metatileset::uses_tile(int idx) const {
	for (size_t i = 0; i < _num_metatiles; i++) {
		if (_metatiles[i]->uses_tile(idx)) {
			return true;
		}
	}
	return false;
}

void Metatileset::trim_tileset() {
	for (int i = 0; i < MAX_NUM_TILES; i++) {
		Deep_Tile *dt = _tileset.tile(i);
		if (!dt->undefined() && dt->is_blank() && !uses_tile(i)) {
			dt->undefined(true);
		}
	}
}

/**
 * Parse contents of a .bin file (metatileset content) and create metatiles to make a metatileset
 */
Metatileset::Result Metatileset::read_metatiles(const uint8_t *filePtr, size_t size) {
  if (!_tileset.num_tiles()) { 
    std::cout << "No corresponding graphics file chosen." << std::endl;
    return (_result = Result::META_NO_GFX);
  } // no graphics

  const unsigned char *ptr = filePtr;
  const unsigned char *endPtr = filePtr+ size;

  while (ptr < endPtr) {
    unsigned char data[METATILE_SIZE * METATILE_SIZE] = {};

    // Read a metatile's worth of data
    size_t bytesToRead = std::min<size_t>(METATILE_SIZE * METATILE_SIZE, endPtr - ptr);
    std::memcpy(data, ptr, bytesToRead);
    ptr += bytesToRead;

    // Error and bad-file handling
    if (bytesToRead < METATILE_SIZE * METATILE_SIZE) { return (_result = Result::META_TOO_SHORT); }
    if (_num_metatiles == MAX_NUM_METATILES) { return (_result = Result::META_TOO_LONG); }

    Metatile *mt = _metatiles[_num_metatiles++];
    for (int y = 0; y < METATILE_SIZE; y++) {
      for (int x = 0; x < METATILE_SIZE; x++) {
        unsigned char v = data[y * METATILE_SIZE + x];
        mt->offset(x, y, v);
      }
    }
  }

  std::cout << "Number of metatiles: " << _num_metatiles << std::endl;

  return (_result = Result::META_OK);
}



unsigned char *Metatileset::print_rgb(const Map &map) const {
	int w = map.width(), h = map.height();
	int bw = w * METATILE_PX_SIZE, bh = h * METATILE_PX_SIZE;
	unsigned char *buffer = new unsigned char[bw * bh * NUM_CHANNELS]();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Block *b = map.block((uint8_t)x, (uint8_t)y);
			const Metatile *mt = _metatiles[b->id()];
			for (int ty = 0; ty < METATILE_SIZE; ty++) {
				for (int tx = 0; tx < METATILE_SIZE; tx++) {
					const Tile *t = mt->tile(tx, ty);
					const Deep_Tile *dt = _tileset.const_tile_or_roof(t->index());
					size_t o = ((y * METATILE_SIZE + ty) * bw + x * METATILE_SIZE + tx) * TILE_SIZE * NUM_CHANNELS;
					for (int py = 0; py < TILE_SIZE; py++) {
						int my = t->y_flip() ? TILE_SIZE - py - 1 : py;
						for (int px = 0; px < TILE_SIZE; px++) {
							int mx = t->x_flip() ? TILE_SIZE - px - 1 : px;
							const unsigned char *rgb = dt->const_colored_pixel(t->palette(), mx, my);
							size_t j = o + (py * bw + px) * NUM_CHANNELS;
							buffer[j++] = rgb[0];
							buffer[j++] = rgb[1];
							buffer[j] = rgb[2];
						}
					}
				}
			}
		}
	}
	return buffer;
}

/**
 * Draw a metatile and return image data as base64-encoded PNG
 *
 * @param x X-coordinate of the top-left corner of the metatile
 * @param y Y-coordinate of the top-left corner of the metatile
 * @param id ID of the metatile to draw
 * @param zoom Whether to draw the metatile zoomed in
 *
 * @return Base64-encoded PNG image data
 */
std::string Metatileset::draw_metatile(uint8_t id) const {
  // Tile size = 8
  // METATILE_SIZE = 4
  // IMAGE_SIZE = 32, assuming 32x32. probably needs to be bigger.
  //
  // current status: creates a 32x32 image that represents a 2x2 metatile. we need a 4x4
    std::cout << "Tile size: " << TILE_SIZE << std::endl;
    Metatile *mt = _metatiles[id];

    std::cout << "METATILE_SIZE: " << METATILE_SIZE << std::endl;
    std::cout << "num channels: " << NUM_CHANNELS << std::endl;


    // Create a vector to hold the pixel data
    // metatile final images are 32px x 32px in size. but each metatile is 4x4 tiles, so we need to draw 4 tiles in each direction
    std::vector<unsigned char> image_data(32*32* NUM_CHANNELS, 255); // Initialize with white background

    std::cout << "created image data" << std::endl;

    // loop through each tile in the metatile
    for (int ty = 0; ty < 4; ty++) {
        for (int tx = 0; tx < 4; tx++) {
            std::cout << "drawing tile " << tx << ", " << ty << std::endl;

            int src_x = tx * TILE_SIZE;
            int src_y = ty * TILE_SIZE;

            // Grab the first tile in the metatile
            const Tile *tile = mt->tile(tx, ty);
            //std::cout << "tile index: " << tile->index() << std::endl;

            // Get the deep tile info (from the tileset png)
            const Deep_Tile *deepTile = _tileset.const_tile_or_roof(tile->index());

            // Get the deeptile with palette applied
            const unsigned char *buffer = deepTile->rgb(tile->palette());

            // Copy tile data into image_data
            for (int dy = 0; dy < 8; dy++) {
                for (int dx = 0; dx < 8; dx++) {
                    // Calculate position in image_data for this pixel
                    int dest_x = tx * TILE_SIZE + dx;
                    int dest_y = ty * TILE_SIZE + dy;

                    // 32 because metatile printed is 32x32
                    int dest_pos = (dest_y * 32 + dest_x) * NUM_CHANNELS;

                    // Mod the source x and y by 8 to wrap around the tile, bc each deeptile/tile is 8x8
                    int src_pos = (((src_y + dy)%8) * LINE_BYTES + ((src_x + dx)%8) * NUM_CHANNELS);

                      // Debugging output
                    //std::cout << "Copying pixel: (" << dx << ", " << dy << ") ";
                    //std::cout << "from (" << src_x + dx << ", " << src_y + dy << ") ";
                    //std::cout << "to (" << dest_x << ", " << dest_y << ") ";
                    //std::cout << "src_pos: " << src_pos << " dest_pos: " << dest_pos << std::endl;


                    std::memcpy(&image_data[dest_pos], &buffer[src_pos], NUM_CHANNELS);
                }
            }
        }
    }

    std::cout << "copied tile data into image data" << std::endl;

    int imageHeight = 32; // need, or similar
    int imageWidth = 32; // need, or similar
    //int imageHeight = s; 
    //int imageWidth = s; 
    // Encode image data as PNG and return base64-encoded string
    std::vector<unsigned char> png_buffer;
    bool result = Png::encode_png(png_buffer, image_data.data(), imageWidth, imageHeight, NUM_CHANNELS == 4);

    if (!result) {
      std::cerr << "Failed to encode PNG" << std::endl;
      return "";
    }

    // Return base64-encoded PNG
    return Png::base64_encode(png_buffer.data(), png_buffer.size());
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
