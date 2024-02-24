#include <iostream>
#include <png.h>
#include "parse-png.h"
#include <vector>

Png::Png(PngData pngData) : _valid(false), _size(0) {

  std::cout << "In PNG" << std::endl;
  _valid = parsePng(pngData.buf, pngData.size);
  std::cout << "valid: " << _valid << std::endl;

  if (_valid) {
    _bufferPtr = pngData.buf;
    _size = pngData.size;
  }
}

Png::~Png() {
  // PNG data is freed in JS.
  // This deconstructor only here for posterity
}

/**
 * Get the value of the PNG data at the given index
 *
 * @param index Index of the PNG data
 *
 * @return Value of the PNG data at the given index
 */
char Png::valueAt(int index) {
  if (index < 0 || index >= _size) {
    std::cerr << "Index out of range" << std::endl;
    return 0;
  }

  return _bufferPtr[index];
}

/**
 * Parse png data from buffer in memory. If valid, saves width, height, and bit depth to class members.
 *
 * @param bufferPtr Pointer to the PNG file data
 * @param bufferSize Size of the PNG file data
 *
 * @return True if the PNG data was parsed successfully, false otherwise
 */
bool Png::parsePng(uint8_t* bufferPtr, size_t bufferSize) {
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


  // Get image width and height
  _width = png_get_image_width(png_ptr, info_ptr);
  _height = png_get_image_height(png_ptr, info_ptr);

  std::cout << "Width: " << _width << ", Height: " << _height << std::endl;

  // Get bit depth
  _depth = png_get_bit_depth(png_ptr, info_ptr);
  std::cout << "Depth: " << _depth << std::endl;


  // If image is already grayscale
  bool isGrayscale = png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_GRAY;
  std::cout << "Grayscale: " << isGrayscale << "color type: " << (int) png_get_color_type(png_ptr, info_ptr) << std::endl;

  // If image is not grayscale, desaturate it
  if (!isGrayscale) {
    // TODO
  }

  // Clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  return true;
}

/**
 * Desaturate a row of PNG data
 *
 * @param row Pointer to the row of PNG data
 * @param width Width of the row
 */
void Png::desaturate(png_bytep row, int width, int channels) {
  for (int x = 0; x < width; x++) {
    // Extract color components based on the number of channels
    png_byte color[4] = {0}; // Initialize array to hold color components
    for (int c = 0; c < channels; c++) {
      color[c] = row[x * channels + c];
    }

    // Compute luminance (grayscale) value based on color components
    // Adjust weights based on the color model (RGB, RGBA, grayscale, etc.)
    png_byte gray = 0;
    if (channels == 1) { // Grayscale image
      gray = color[0]; // Use the single component value directly
    } else { // RGB, RGBA, or other color models
      gray = (png_byte)(0.2126 * color[0] + 0.7152 * color[1] + 0.0722 * color[2]);
    }

    // Set all color components to grayscale value
    for (int c = 0; c < channels; c++) {
      row[x * channels + c] = gray;
    }
  }
}

/**
 * Returns number of channels (color type) in the PNG data
 */
int Png::count() {
  // Determine the number of channels based on the depth
  if (_depth == 1 || _depth == 2)
    return 1; // Binary image with 1 channel
  else if (_depth == 8)
    return 3; // RGB image with 3 channels
  else if (_depth == 16)
    return 3; // RGB image with 3 channels
  else if (_depth == 32)
    return 4; // RGBA image with 4 channels
  else
    return 0; // Unknown or unsupported depth
}
