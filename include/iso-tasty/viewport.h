#ifndef ISOTASTY_VIEWPORT_H
#define ISOTASTY_VIEWPORT_H

#include "iso-tasty/renderer.h"
#include "iso-tasty/map.h"

namespace IsoTasty {
  class Viewport {
  public:
    // Create a viewport of the world given a size
    Viewport(unsigned int width, unsigned int height);

    // Query the size of the viewport
    unsigned int width();
    unsigned int height();

    // Manipulate the viewport focus
    void move(double deltaX, double deltaZ);
    void rotate(double delta);
    void zoom(double factor);

    // Draws the world as seen by this viewport
    void draw(Renderer* renderer, Map* map);

  private:
    // The rotation of the world
    double _rotation;

    // The vector from the center of the world to the center of the viewport
    double _x;
    double _z;

    // The amount to zoom
    double _zoom;

    // The size of the viewport
    unsigned int _width;
    unsigned int _height;
  };
}

#endif