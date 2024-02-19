#include <iostream>
#include "map/map.h"
#include <emscripten.h>
#include "state.h"
#include "utils/filereader.h"
#include <emscripten/bind.h>

AppState state;

extern "C" {
  EMSCRIPTEN_KEEPALIVE

  int main(int argc, char **argv) {
    EM_ASM(
      Module['print'] = function(text) {
        console.log(text);
      };
    );

    std::cout << "Initialized" << std::endl;
    Map map = state.getMap();
    map.size(2, 1);
    // Initialize settings
    // Initialize map
    // Initialize blocks
    std::cout << "Map Width: " << static_cast<int>(map.width()) << std::endl;
    std::cout << "Map Height: " << static_cast<int>(map.height())  << std::endl;

    return 0;
  }
}
