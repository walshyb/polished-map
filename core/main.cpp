#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"
#include "utils/filereader.h"
#include <emscripten/bind.h>
#include <png.h>

AppState *state;


int main(int argc, char **argv) {
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    std::cerr << "Failed to create PNG read struct" << std::endl;
    return 1;
  }

  EM_ASM(
    Module['print'] = function(text) {
      console.log(text);
    };
  );

  std::cout << "Initialized" << std::endl;

  return 0;
}

