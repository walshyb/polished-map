#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <stdint.h>
#include <limits>
#include <cmath>
#include <string>
#include <string_view>

#ifdef _WIN32
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif

#ifdef _WIN32
#define FILL(a, v, n) \
	__pragma(warning(push)) \
	__pragma(warning(disable:4127)) \
	do { \
		for (size_t __fill_i_ = 0; __fill_i_ < (size_t)(n); __fill_i_++) { \
			a[__fill_i_] = (v); \
		} \
	} while (0) \
	__pragma(warning(pop))
#else
#define FILL(a, v, n) \
	do { \
		for (size_t __fill_i_ = 0; __fill_i_ < (size_t)(n); __fill_i_++) { \
			a[__fill_i_] = (v); \
		} \
	} while (0)
#endif

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(a[0]))
#endif

typedef uint8_t size8_t;
typedef uint16_t size16_t;
typedef uint32_t size32_t;
typedef uint64_t size64_t;

extern const std::string whitespace;

bool starts_with(std::string_view s, std::string_view p);
bool ends_with(std::string_view s, std::string_view p);
bool ends_with(std::wstring_view s, std::wstring_view p);
void trim(std::string &s, const std::string &t = whitespace);
void lowercase(std::string &s);
void remove_comment(std::string &s, char c = ';');
void remove_dot_ext(const char *f, char *s);
void add_dot_ext(const char *f, const char *ext, char *s);
int text_width(const char *l, int pad);
int text_width(const char *l, int pad = 0);
bool file_exists(const char *f);
size_t file_size(const char *f);

#endif
