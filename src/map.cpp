#include "iso-tasty/map.h"

#include <cstddef>
#include <string.h>
#include <math.h>

#include <stdio.h>
#include <Windows.h>

// Struct for the point data. Should put somewhere.
struct Point {
  float x;
  float y;
  float z;
};

// Helper function for controlPoints()
static Point* firstControlPoints(Point* rhs, Point* first, unsigned int num) {
  Point* tmp = new Point[num];

  float bx = 2.0f;
  float by = 2.0f;
  first[0].x = rhs[0].x / bx;
  first[0].y = rhs[0].y / by;
  for (unsigned int i = 1; i < num; i++) {
    tmp[i].x = 1.0f / bx;
    tmp[i].y = 1.0f / by;
    if (i < num - 1) {
      bx = 4.0f - tmp[i].x;
      by = 4.0f - tmp[i].y;
    }
    else {
      bx = 3.5f - tmp[i].x;
      by = 3.5f - tmp[i].y;
    }
    first[i].x = (rhs[i].x - first[i - 1].x) / bx;
    first[i].y = (rhs[i].y - first[i - 1].y) / by;
  }
  for (unsigned int i = 1; i < num; i++) {
    first[num - i - 1].x -= tmp[num - i].x * first[num - i].x;
    first[num - i - 1].y -= tmp[num - i].y * first[num - i].y;
  }
  delete [] tmp;
  return first;
}

// This is a routine that gives you control points for a smooth curve through the given
// points. It is based on solving the derivative of the cubic bezier routine.

// It was based upon my own math and the code of http://www.codeproject.com/KB/graphics/BezierSpline.aspx
// Adapted for 3d points and C++ by myself.
static void controlPoints(Point* points, Point* first, Point* second, unsigned int num) {

  if (points == NULL) {
    return;
  }

  unsigned int n = num - 1;

  if (n < 1) {
    return;
  }
  else if (n == 1) {
    first[0].x = (2.0f * points[0].x + points[1].x) / 3.0f;
    first[0].y = (2.0f * points[0].y + points[1].y) / 3.0f;

    second[0].x = 2.0f * first[0].x - points[0].x;
    second[0].y = 2.0f * first[0].y - points[0].y;

    return;
  }

  Point* nodes = new Point[n];
  for (unsigned int i = 1; i < n-1; i++) {
    nodes[i].x = 4.0f * points[i].x + 2.0f * points[i + 1].x;
    nodes[i].y = 4.0f * points[i].y + 2.0f * points[i + 1].y;
  }
  nodes[0].x = points[0].x + 2.0f * points[1].x;
  nodes[n - 1].x = (8.0f * points[n - 1].x + points[n].x) / 2.0f;
  nodes[0].y = points[0].y + 2.0f * points[1].y;
  nodes[n - 1].y = (8.0f * points[n - 1].y + points[n].y) / 2.0f;

  firstControlPoints(nodes, first, n);

  for (unsigned int i = 0; i < n; i++) {
    if (i < n - 1) {
      second[i].x = 2.0f * points[i + 1].x - first[i + 1].x;
      second[i].y = 2.0f * points[i + 1].y - first[i + 1].y;
    }
    else {
      second[i].x = (points[n].x + first[n - 1].x) / 2.0f;
      second[i].y = (points[n].y + first[n - 1].y) / 2.0f;
    }
  }
  delete [] nodes;
}

IsoTasty::Map::Map(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _x(width/2), 
  _z(height/2) {
  _map = new Tile*[width * height];
  for (unsigned int x = 0; x < width; x++) {
    for (unsigned int z = 0; z < height; z++) {
      unsigned int offset = (_width * x + z) * 44;
      _map[_width * x + z] = new Tile();
    }
  }
}

IsoTasty::Map::~Map() {
  delete [] _map;
}

unsigned int IsoTasty::Map::width() {
  return _width;
}

unsigned int IsoTasty::Map::height() {
  return _height;
}

IsoTasty::Tile* IsoTasty::Map::atIndex(unsigned int x, unsigned int z) {
  return _map[_width * x + z];
}

unsigned int IsoTasty::Map::x() {
  return _x;
}

unsigned int IsoTasty::Map::x(unsigned int value) {
  _x = value;
  if (_x >= _width) {
    _x = _width-1;
  }
  return _x;
}

unsigned int IsoTasty::Map::z() {
  return _z;
}

unsigned int IsoTasty::Map::z(unsigned int value) {
  _z = value;
  if (_z >= _height) {
    _z = _height-1;
  }
  return _z;
}

void IsoTasty::Map::raise(float amount) {
  // Every tile this touches is raised a little bit
  IsoTasty::Tile* tile;
  float old_height;

  if (_x != _width && _z != _height) {
    // Bottom Right
    tile = atIndex(_x, _z);
    old_height = tile->cornerHeight(IsoTasty::TOP_LEFT);
    tile->cornerHeight(IsoTasty::TOP_LEFT, old_height + amount);
  }

  if (_x != 0 && _z != _height) {
    // Bottom Left
    tile = atIndex(_x-1, _z);
    old_height = tile->cornerHeight(IsoTasty::TOP_RIGHT);
    tile->cornerHeight(IsoTasty::TOP_RIGHT, old_height + amount);
  }

  if (_x != _width && _z != 0) {
    // Top Right
    tile = atIndex(_x, _z-1);
    old_height = tile->cornerHeight(IsoTasty::BOT_LEFT);
    tile->cornerHeight(IsoTasty::BOT_LEFT, old_height + amount);
  }

  if (_x != 0 && _z != 0) {
    // Top Left
    tile = atIndex(_x-1, _z-1);
    old_height = tile->cornerHeight(IsoTasty::BOT_RIGHT);
    tile->cornerHeight(IsoTasty::BOT_RIGHT, old_height + amount);
  }

  // Regenerate curve points along this point

  // Horizontal

  Point* line;
  Point* first;
  Point* second;
  line = new Point[_width+1];
  first = new Point[_width];
  second = new Point[_width];

  for (unsigned int x = 0; x < _width; x++) {
    tile = atIndex(x, _z);
    line[x].x = (float)x;
    line[x].y = tile->cornerHeight(IsoTasty::TOP_LEFT);
    line[x].z = 0.0f; // does not matter
  }
  line[_width].x = (float)_width;
  line[_width].y = tile->cornerHeight(IsoTasty::TOP_RIGHT);
  line[_width].z = 0.0f;

  controlPoints(line, first, second, _width+1);

  // Pass control points back to tiles

  for (unsigned int x = 0; x < _width; x++) {
    tile = atIndex(x, _z);
    tile->firstControl(IsoTasty::TOP_LEFT, first[x].y);
    tile->secondControl(IsoTasty::TOP_LEFT, second[x].y);
    if (_z != 0) {
      tile = atIndex(x, _z-1);
      tile->firstControl(IsoTasty::BOT_RIGHT, first[x].y);
      tile->secondControl(IsoTasty::BOT_RIGHT, second[x].y);
    }
  }

  delete [] first;
  delete [] second;
  delete [] line;

  // Vertical

  line = new Point[_height+1];
  first = new Point[_height+1];
  second = new Point[_height+1];

  for (unsigned int z = 0; z < _height; z++) {
    tile = atIndex(_x, z);
    line[z].x = (float)z; // does not matter
    line[z].y = tile->cornerHeight(IsoTasty::TOP_LEFT);
    line[z].z = 0.0f;
  }
  line[_height].x = (float)_height;
  line[_height].y = tile->cornerHeight(IsoTasty::BOT_LEFT);
  line[_height].z = 0.0f;

  controlPoints(line, first, second, _height+1);

  // Pass control points back to tiles

  for (unsigned int z = 0; z < _height; z++) {
    tile = atIndex(_x, z);
    tile->firstControl(IsoTasty::BOT_LEFT, first[z].y);
    tile->secondControl(IsoTasty::BOT_LEFT, second[z].y);
    if (_x != 0) {
      tile = atIndex(_x-1, z);
      tile->firstControl(IsoTasty::TOP_RIGHT, first[z].y);
      tile->secondControl(IsoTasty::TOP_RIGHT, second[z].y);
    }
  }

  delete [] first;
  delete [] second;
  delete [] line;
}

void IsoTasty::Map::lift(float amount) {
  // Just the one tile is raised at all corners
  IsoTasty::Tile* tile;

  tile = atIndex(_x, _z);
  for (unsigned int i = 0; i < 4; i++) {
    tile->cornerHeight(i, tile->cornerHeight(i) + amount);
    tile->firstControl(i, tile->firstControl(i) + amount);
    tile->secondControl(i, tile->secondControl(i) + amount);
  }
}
