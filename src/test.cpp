#include "iso-tasty.h"

#define WIDTH 960
#define HEIGHT 540

#include <Windows.h>

int main(int argc, char** argv) {
  // initialize Apsis
  Apsis::VideoSettings settings;
  settings.resolutionX = WIDTH;
  settings.resolutionY = HEIGHT;
  try {
    Apsis::Engine* engine = new Apsis::Engine(&settings);
    engine->run();
  }
  catch (char* msg) {
    OutputDebugStringA("*** Exception ***\n");
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
    OutputDebugStringA("*** Exception ***\n");
  }

  return 0;
}