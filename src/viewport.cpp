#include "iso-tasty/viewport.h"

#include "stdio.h"
#include "math.h"

IsoTasty::Viewport::Viewport(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _rotation(0.0),
  _x(0.0),
  _z(0.0),
  _zoom(0.25) {
}

unsigned int IsoTasty::Viewport::width() {
  return _width;
}

unsigned int IsoTasty::Viewport::height() {
  return _height;
}

float cubicBezier(float p0, float p1, float p2, float p3, float t) {
  float co1 = t;
  float co1_2 = t*t;
  float co1_3 = t*t*t;

  float co2 = (1-t);
  float co2_2 = (1-t)*(1-t);
  float co2_3 = (1-t)*(1-t)*(1-t);

  return co2_3*p3 + 3*co1*co2_2*p2 + 3*co1_2*co2*p1 + co1_3*p0;
}

void IsoTasty::Viewport::draw(Renderer* renderer, Map* map) {
  renderer->setProjection(_width, _height, false, _rotation, _x, _z, _zoom);

  float half_height = map->height() / 2.0f;
  float half_width = map->width() / 2.0f;

  for (unsigned int z = 0; z < map->height(); z++) {
    for (unsigned int x = 0; x < map->width(); x++) {
      IsoTasty::Tile* tile = map->atIndex(x, z);
      float h = 1.0f;
      float top = tile->hover();
      float heights[4];
      for (int i = 0; i < 4; i++) {
        heights[i] = tile->cornerHeight(i);
      }

      float p0 = tile->cornerHeight(IsoTasty::TOP_LEFT);
      float p1 = tile->firstControl(IsoTasty::TOP_LEFT);
      float p2 = tile->secondControl(IsoTasty::TOP_LEFT);
      float p3 = tile->cornerHeight(IsoTasty::TOP_RIGHT);

      float first_y = cubicBezier(p0,p1,p2,p3,0.33f);
      float second_y = cubicBezier(p0,p1,p2,p3,0.67f);
      float firsts[4] = {5.0f, 5.0f, 5.0f, 5.0f};
      float seconds[4] = {5.0f, 5.0f, 5.0f, 5.0f};
      firsts[IsoTasty::TOP_LEFT] = first_y;
      seconds[IsoTasty::TOP_LEFT] = second_y;

      p0 = tile->cornerHeight(IsoTasty::BOT_LEFT);
      p1 = tile->firstControl(IsoTasty::BOT_RIGHT);
      p2 = tile->secondControl(IsoTasty::BOT_RIGHT);
      p3 = tile->cornerHeight(IsoTasty::BOT_RIGHT);
      first_y = cubicBezier(p0,p1,p2,p3,0.33f);
      second_y = cubicBezier(p0,p1,p2,p3,0.67f);
      firsts[IsoTasty::BOT_RIGHT] = first_y;
      seconds[IsoTasty::BOT_RIGHT] = second_y;

      p0 = tile->cornerHeight(IsoTasty::TOP_RIGHT);
      p1 = tile->firstControl(IsoTasty::TOP_RIGHT);
      p2 = tile->secondControl(IsoTasty::TOP_RIGHT);
      p3 = tile->cornerHeight(IsoTasty::BOT_RIGHT);
      first_y = cubicBezier(p0,p1,p2,p3,0.33f);
      second_y = cubicBezier(p0,p1,p2,p3,0.67f);
      firsts[IsoTasty::TOP_RIGHT] = first_y;
      seconds[IsoTasty::TOP_RIGHT] = second_y;

      p0 = tile->cornerHeight(IsoTasty::TOP_LEFT);
      p1 = tile->firstControl(IsoTasty::BOT_LEFT);
      p2 = tile->secondControl(IsoTasty::BOT_LEFT);
      p3 = tile->cornerHeight(IsoTasty::BOT_LEFT);
      first_y = cubicBezier(p0,p1,p2,p3,0.33f);
      second_y = cubicBezier(p0,p1,p2,p3,0.67f);
      firsts[IsoTasty::BOT_LEFT] = first_y;
      seconds[IsoTasty::BOT_LEFT] = second_y;

      //renderer->drawTile((float)x - half_width, -top, (float)z - half_height, 0.5f, 0.5f, 0.5f, heights, first_y, second_y);

      renderer->drawTile((float)(x - half_width), (float)(-3 * (1 / _zoom) - top), (float)(z - half_height), 0.5f, 0.5f, 0.5f, heights, firsts, seconds);
      renderer->drawTileTop((float)(x - half_width), (float)(-3 * (1 / _zoom) - top), (float)(z - half_height), 0.5f, 0.5f, 0.5f, heights, firsts, seconds);
    }
  }

  IsoTasty::Tile* tile = map->atIndex(map->x(), map->z());
  renderer->drawSphere(
    (float)(map->x() - half_width - 0.5f), 
    (float)(-3 * (1 / _zoom) + (-tile->hover() + tile->cornerHeight(IsoTasty::TOP_LEFT))/2.0f),
    (float)(map->z() - half_height - 0.5f),
    0.25f, 0.25f, 0.25f);

  //renderer->test();
}

void IsoTasty::Viewport::move(double deltaX, double deltaZ) {
  double radians = _rotation / 180.0 * 3.1415926;
  double cosine = cos(radians);
  double sine = sin(radians);

  // Move respective of rotation
  _x += deltaX*cosine - deltaZ*sine;
  _z += deltaX*sine + deltaZ*cosine;

  //_x += deltaX;
  //_z += deltaZ;
}

void IsoTasty::Viewport::rotate(double delta) {
  if (delta < 0) {
    delta = -fmod(-delta, 360);
  }
  _rotation += 360.0;
  _rotation += delta;
  _rotation = fmod(_rotation, 360.0);
}

void IsoTasty::Viewport::zoom(double factor) {
  _zoom *= factor;
}