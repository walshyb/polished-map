#include <iostream>
#include <png.h>
#include "parse-png.h"

bool getPngData(const uint8_t* bufferPtr, size_t bufferSize, int& width, int& height, int& depth) {
  png_structp png_ptr;
  png_infop info_ptr;

  // Create PNG structures
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    std::cerr << "Error creating PNG read struct" << std::endl;
    return false;
  }

  // Set up error handling
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    std::cerr << "Error creating PNG info struct" << std::endl;
    return false;
  }

  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    std::cerr << "Error during PNG read" << std::endl;
    return false;
  }

  // Set up custom read function
  PngData png_data_info;
  png_data_info.buf = bufferPtr;
  png_data_info.size = bufferSize;
  png_data_info.pos = 0;

  // This is a lambda function that will be called by libpng to read data from the PNG file
  // the custom read function will copy data from the PNG file data buffer to the libpng buffer
  png_set_read_fn(png_ptr, &png_data_info, [](png_structp png_ptr, png_bytep data, png_size_t length) {
    // Get pointer to PNG data
    PngData* png_data_info = (PngData*)png_get_io_ptr(png_ptr);

    // Check if we have enough data
    if (png_data_info->pos + length > png_data_info->size) {
      std::cerr << "Error reading PNG data" << std::endl;
      return;
    }

    // Copy data to the buffer
    memcpy(data, png_data_info->buf + png_data_info->pos, length);

    // Update position
    png_data_info->pos += length;
  });

  // Read PNG header
  png_read_info(png_ptr, info_ptr);

  // Get bit depth
  depth = png_get_bit_depth(png_ptr, info_ptr);
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);

  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  return true;
}


