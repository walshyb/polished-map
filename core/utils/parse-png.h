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
