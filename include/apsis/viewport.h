#ifndef APSIS_VIEWPORT_H
#define APSIS_VIEWPORT_H

#include "apsis/renderer.h"

#include "apsis/primitives/camera.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"
#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"
#include "apsis/primitives/texture.h"

#include "apsis/model/thing.h"

#include "apsis/world/terrain.h"

namespace Apsis {
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
    void draw(Renderer* renderer, World::Terrain* map);

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

    std::vector<Model::Thing> _things;
    std::vector<Primitives::Camera> _cameras;

    World::Terrain* _terrain;
  };
}

#endif