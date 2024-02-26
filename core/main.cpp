#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"
#include "utils/filereader.h"
#include <emscripten/bind.h>

AppState *state;


int main(int argc, char **argv) {
  EM_ASM(
    Module['print'] = function(text) {
      console.log(text);
    };
  );

  std::cout << "Initialized" << std::endl;

  return 0;
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE

  std::string drawMetatile(int id) {
    Metatileset *metatileset = state->getMetatileset();
    std::cout << "Metatileset: " << metatileset << std::endl;
    std::string image = metatileset->draw_metatile(0, 0, 1, false);
    std::cout << "Image: " << image << std::endl;
    return image;
  }
}
