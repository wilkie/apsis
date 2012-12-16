#include "iso-tasty.h"

#define WIDTH 960
#define HEIGHT 540

int main(int argc, char** argv) {
  // initialize engine
  IsoTasty::VideoSettings settings;
  settings.resolutionX = WIDTH;
  settings.resolutionY = HEIGHT;
  IsoTasty::Engine* engine = new IsoTasty::Engine(&settings);
  engine->run();

  return 0;
}