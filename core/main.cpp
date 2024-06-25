#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include <emscripten/bind.h>
#include "state.h"
#include "utils/filereader.h"

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
    AppState *appState = &AppState::getInstance();
    Metatileset *metatileset = appState->getMetatileset();
    std::cout << "Metatileset: " << metatileset << std::endl;
    std::string image = metatileset->draw_metatile(2);
    std::cout << "Image: " << image << std::endl;
    return image;
  }
}
