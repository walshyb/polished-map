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

