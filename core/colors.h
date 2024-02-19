#ifndef COLORS_H
#define COLORS_H

#include <vector>
#include <array>
#include "utils.h"

#define RGB5C(x) (unsigned char)((x) * 33 / 4) // 5-bit to 8-bit
#define CRGB5(c) (unsigned char)((c) / 8)      // 8-bit to 5-bit

enum class Palettes { MORN, DAY, NITE, DARKNESS, INDOOR, CUSTOM };

enum class Palette { GRAY, RED, GREEN, WATER, YELLOW, BROWN, ROOF, TEXT, MONOCHROME, UNDEFINED,
	PRIORITY_GRAY = 0x80, PRIORITY_RED, PRIORITY_GREEN, PRIORITY_WATER,
	PRIORITY_YELLOW, PRIORITY_BROWN, PRIORITY_ROOF, PRIORITY_TEXT };

enum class Hue { WHITE, DARK, LIGHT, BLACK };

enum class Roof_Palettes { ROOF_CUSTOM, ROOF_DAY_NITE, ROOF_MORN_DAY_NITE,
	ROOF_DAY_NITE_CUSTOM, ROOF_MORN_DAY_NITE_CUSTOM };

#define NUM_PALETTE_SETS 6
#define NUM_GAME_PALETTES 8
#define NUM_HUES 4
#define NUM_CHANNELS 3
#define NUM_ROOF_PALETTES 5

typedef std::array<unsigned char, NUM_CHANNELS> ColorArray;
typedef std::array<ColorArray, NUM_HUES> HueArray;
typedef std::vector<HueArray> PalVec;

class Color {
private:
	static void color(Palettes l, Palette p, Hue h, ColorArray v);
	static void color(Palettes l, Palette p, HueArray v);
	static unsigned char *colors(Palettes l, Palette p, Hue h);
public:
	static unsigned char desaturated(unsigned char r, unsigned char g, unsigned char b);
	static Hue ordered_hue(int i);
	static unsigned char hue_mono(Hue h);
	static Hue mono_hue(unsigned char c);
	static const unsigned char *color(Palettes l, Palette p, Hue h);
	static PalVec pajse_palettes(const char *f);
	static Palettes read_palettes(const char *f, Palettes pals);
	static bool read_roof_colors(const char *f, uint8_t map_group, Roof_Palettes roof_palettes);
	static bool write_palettes(const char *f);
};

#endif
