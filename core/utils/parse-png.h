#ifndef PARSE_PNG_H
#define PARSE_PNG_H

#include <iostream>
#include <png.h>

// Since we are using a custom read function,
// and reading PNG data from memory,
// we need to store the PNG data in a struct
struct PngData {
  uint8_t* buf;   // Pointer to the PNG file data
  size_t size;          // Size of the PNG file data
  size_t pos;           // Current position in the data buffer
  const char* filename; // Name of the PNG file
};

class Png {
public:
  Png(PngData pngData);
  ~Png();
  bool valid() const { return _valid; }
  int width() const { return _width; }
  int height() const { return _height; }
  int depth() const { return _depth; }
  size_t size() const { return _size; }
  char valueAt(int index);
  //void desaturate();
  int count();
  static bool encode_png(std::vector<unsigned char>& out_png, const unsigned char* image_data, int width, int height, bool has_alpha);
  static std::string base64_encode(const unsigned char* buffer, size_t length);
private:
  uint8_t* _bufferPtr;
  int _width, _height, _depth;
  size_t _size;
  bool _valid;
  bool parsePng(uint8_t* bufferPtr, size_t bufferSize);
  unsigned char luminance();
  void desaturate(png_bytep row, int width, int channels);
};


#endif
