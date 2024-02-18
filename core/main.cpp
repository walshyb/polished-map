#include <iostream>
#include "map/map.h"
#include <emscripten.h>

int main(int argc, char **argv) {
  EM_ASM(
    Module['print'] = function(text) {
      console.log(text);
    };
  );

  std::cout << "Initialized" << std::endl;
  // Initialize settings
  // Initialize map
  // Initialize blocks
  Map map = Map();

  return 0;

}

