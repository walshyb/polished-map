#include <iostream>

bool getPngData(const uint8_t* bufferPtr, size_t bufferSize, int& width, int& height, int& depth);

// Since we are using a custom read function,
// and reading PNG data from memory,
// we need to store the PNG data in a struct
struct PngData {
  const uint8_t* buf;   // Pointer to the PNG file data
  size_t size;          // Size of the PNG file data
  size_t pos;           // Current position in the data buffer
};
