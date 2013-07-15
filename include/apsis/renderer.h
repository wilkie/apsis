#ifndef APSIS_RENDERER_H
#define APSIS_RENDERER_H

#include "apsis/world/tile.h"

namespace Apsis {
  class Renderer {
  public:
    Renderer();
    bool initializeViewport(unsigned int width, unsigned int height);
    void setProjection(unsigned int width, unsigned int height, bool orthographic,
      double rotation, double translationX, double translationZ, double zoom);
    void drawCube(float x, float y, float z, float width, float height, float depth);
    void drawSquare(float x, float y, float z, float width, float depth);
    void drawTile(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]);
    void drawTileTop(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]);
    void drawArrays(const float vertices[], const float normals[], const float colors[], const unsigned char indices[], unsigned int num);
    void drawArrays(const double vertices[],
                    const double normals[],
                    const unsigned char indices[],
                    const double texture_coords[],
                    unsigned int num);
    void drawSphere(float x, float y, float z, float width, float height, float depth);

    void drawSquare(double x, double y,
                    double width, double height,
                    double tu, double tv,
                    double tw, double th,
                    double depth);
    void bindTexture(unsigned int texture);

    void clear();

    void test();

  private:
  };
}

#endif