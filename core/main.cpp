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


  /**
   * Create a 32x32 png of a metatile with a specific ID.
   *
   * @param int id the id
   *
   * @return pointer to a character array of the base64 encoded image
   */
  char* drawMetatile(int id) {
    AppState *appState = &AppState::getInstance();
    Metatileset *metatileset = appState->getMetatileset();
    std::cout << "Metatileset: " << metatileset << std::endl;
    std::string image = metatileset->draw_metatile(id);
    std::cout << "Image: " << image << std::endl;


    char* imageArr = new char[image.length() + 1];
    std::strcpy(imageArr, image.c_str());
    return imageArr;
  }

  int getMetatilesetSize() {
    AppState *appState = &AppState::getInstance();
    Metatileset *metatileset = appState->getMetatileset();
    return metatileset->size();
  }
}
