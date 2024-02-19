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

  state = &AppState::getInstance();
  state->getMap()->size(28, 18);

  std::cout << "Width: " << static_cast<int>(state->getMap()->width()) << std::endl;

  return 0;
}

