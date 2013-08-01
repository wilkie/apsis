#include "apsis/agent/impeders/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/input/input_engine.h"

#include <math.h>

Apsis::Agent::Impeders::MapCollider::MapCollider(Apsis::World::Map* map)
  : _map(map) {
}

bool Apsis::Agent::Impeders::MapCollider::update(std::set<unsigned int>& states,
                                                 Apsis::Geometry::Rectangle& from,
                                                 Apsis::Geometry::Point& to) {
  // For every point px, py in the set of 4 defined by given Rectangle
  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  double halfWidth  = ceil(from.width  / 2.0);
  double halfHeight = ceil(from.height / 2.0);

  points[0].x = from.x - halfWidth;
  points[0].y = from.y - halfHeight;
  toPoints[0].x = to.x - halfWidth;
  toPoints[0].y = to.y - halfHeight;

  points[1].x = from.x + halfWidth;
  points[1].y = from.y - halfHeight;
  toPoints[1].x = to.x + halfWidth;
  toPoints[1].y = to.y - halfHeight;

  points[2].x = from.x + halfWidth;
  points[2].y = from.y + halfHeight;
  toPoints[2].x = to.x + halfWidth;
  toPoints[2].y = to.y + halfHeight;

  points[3].x = from.x - halfWidth;
  points[3].y = from.y + halfHeight;
  toPoints[3].x = to.x - halfWidth;
  toPoints[3].y = to.y + halfHeight;

  Apsis::Geometry::Line vectors[4];
  for (int i = 0; i < 4; i++) {
    vectors[i].points[0] = points[i];
    vectors[i].points[1] = toPoints[i];
  }

  if (!_canFall(from)) {
    states.insert(Apsis::State::COLLIDE_DOWN_WITH_MAP);
  }

  if (!_canJump(from)) {
    states.insert(Apsis::State::COLLIDE_UP_WITH_MAP);
  }

  if (_againstWall(from, true)) {
    states.insert(Apsis::State::COLLIDE_LEFT_WITH_MAP);
  }

  if (_againstWall(from, false)) {
    states.insert(Apsis::State::COLLIDE_RIGHT_WITH_MAP);
  }

  // Assume we can move the entire way
  double t = 1.0;

  Apsis::Geometry::Point calculatedPoint;

  // determine if there is an intersection with the map
  for (unsigned int x = 0; x < _map->width(); x++) {
    for (unsigned int y = 0; y < _map->height(); y++) {
      Apsis::World::Tile* tile = _map->tile(x, y);
      if (tile->passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Rectangle tileRect;
      tileRect.x = (x * _map->tileWidth()) + (_map->tileWidth() / 2.0f);
      tileRect.y = (y * _map->tileHeight()) + (_map->tileHeight() / 2.0f);

      tileRect.width  = _map->tileWidth();
      tileRect.height = _map->tileHeight();

      for (int i = 0; i < 4; i++) {
        double tMin, tMax;
        Apsis::Geometry::Line l = vectors[i];
        unsigned int edge = tileRect.clip(&l, &tMin, &tMax);
        if (edge > 0) {
          if (edge == 3) {
            if (!(tMin == 0.0 || tMax == 1.0)) {
              //states.insert(Apsis::State::COLLIDE_DOWN_WITH_MAP);
            }
          }

          if (tMin == 0.0 && tMax == 0.0) { continue; }
          if (tMin < t) {
            // New t value is the amount we will walk down the vector
            t = tMin;

            calculatedPoint = l.points[0];
            if (i == 0) {
              calculatedPoint.x += halfWidth;
              calculatedPoint.y += halfHeight;
            }
            else if (i == 1) {
              calculatedPoint.x -= halfWidth;
              calculatedPoint.y += halfHeight;
            }
            else if (i == 2) {
              calculatedPoint.x -= halfWidth;
              calculatedPoint.y -= halfHeight;
            }
            else if (i == 3) {
              calculatedPoint.x += halfWidth;
              calculatedPoint.y -= halfHeight;
            }
          }
        }
      }
    }
  }

  if (t == 1.0) {
    return true;
  }

  // Get the actual point we should move to
  to = calculatedPoint;
  return false;
}

bool Apsis::Agent::Impeders::MapCollider::_againstWall(Apsis::Geometry::Rectangle& from, bool checkLeft) {
  if (_map == NULL) {
    return false;
  }

  Apsis::Geometry::Rectangle to = from;

  if (checkLeft) {
    to.x -= 1.0f;
  }
  else {
    to.x += 1.0f;
  }

  // For every point px, py in the set of 4 defined by given Rectangle
  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  from.points(points);
  to.points(toPoints);

  Apsis::Geometry::Line vectors[4];
  for (int i = 0; i < 4; i++) {
    vectors[i].points[0] = points[i];
    vectors[i].points[1] = toPoints[i];
  }

  Apsis::Geometry::Point p;

  // determine if there is an intersection with the map
  for (unsigned int x = 0; x < _map->width(); x++) {
    for (unsigned int y = 0; y < _map->height(); y++) {
      Apsis::World::Tile* tile = _map->tile(x, y);
      if (tile->passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Line tileLine;
      tileLine.points[0].x = (x * _map->tileWidth());
      tileLine.points[0].y = (y * _map->tileHeight());
      tileLine.points[1].x = (x * _map->tileWidth());
      tileLine.points[1].y = (y * _map->tileHeight()) + _map->tileHeight();

      if (checkLeft) {
        tileLine.points[0].x += _map->tileWidth();
        tileLine.points[1].x += _map->tileWidth();
      }

      for (int i = 0; i < 4; i++) {
        if (tileLine.clips(&vectors[i], &p)) {
          if (p.y != tileLine.points[0].y &&
              p.y != tileLine.points[1].y) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

bool Apsis::Agent::Impeders::MapCollider::_canFall(Apsis::Geometry::Rectangle& from) {
  if (_map == NULL) {
    return true;
  }

  Apsis::Geometry::Rectangle to = from;
  to.y += 1.0;

  // For every point px, py in the set of 4 defined by given Rectangle
  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  from.points(points);
  to.points(toPoints);

  Apsis::Geometry::Line vectors[4];
  for (int i = 0; i < 4; i++) {
    vectors[i].points[0] = points[i];
    vectors[i].points[1] = toPoints[i];
  }

  Apsis::Geometry::Point p;

  // determine if there is an intersection with the map
  for (unsigned int x = 0; x < _map->width(); x++) {
    for (unsigned int y = 0; y < _map->height(); y++) {
      Apsis::World::Tile* tile = _map->tile(x, y);
      if (tile->passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Line tileLine;
      tileLine.points[0].x = (x * _map->tileWidth());
      tileLine.points[0].y = (y * _map->tileHeight());
      tileLine.points[1].x = (x * _map->tileWidth()) + _map->tileWidth();
      tileLine.points[1].y = (y * _map->tileHeight());

      for (int i = 0; i < 4; i++) {
        if (tileLine.clips(&vectors[i], &p)) {
          if (p.x != tileLine.points[0].x &&
              p.x != tileLine.points[1].x) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

bool Apsis::Agent::Impeders::MapCollider::_canJump(Apsis::Geometry::Rectangle& from) {
  if (_map == NULL) {
    return true;
  }

  Apsis::Geometry::Rectangle to = from;
  to.y -= 1.0;

  // For every point px, py in the set of 4 defined by given Rectangle
  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  from.points(points);
  to.points(toPoints);

  Apsis::Geometry::Line vectors[4];
  for (int i = 0; i < 4; i++) {
    vectors[i].points[0] = points[i];
    vectors[i].points[1] = toPoints[i];
  }

  Apsis::Geometry::Point p;

  // determine if there is an intersection with the map
  for (unsigned int x = 0; x < _map->width(); x++) {
    for (unsigned int y = 0; y < _map->height(); y++) {
      Apsis::World::Tile* tile = _map->tile(x, y);
      if (tile->passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Line tileLine;
      tileLine.points[0].x = (x * _map->tileWidth());
      tileLine.points[0].y = (y * _map->tileHeight()) + _map->tileHeight();
      tileLine.points[1].x = (x * _map->tileWidth()) + _map->tileWidth();
      tileLine.points[1].y = (y * _map->tileHeight()) + _map->tileHeight();

      for (int i = 0; i < 4; i++) {
        if (tileLine.clips(&vectors[i], &p)) {
          if (p.x != tileLine.points[0].x &&
              p.x != tileLine.points[1].x) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

const char* Apsis::Agent::Impeders::MapCollider::rule() {
  return "can not move through map walls";
}
