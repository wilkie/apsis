#include "apsis/agent/impeders/map_collider.h"

#include "apsis/geometry/point.h"
#include <math.h>

Apsis::Agent::Impeders::MapCollider::MapCollider(Apsis::World::Map* map)
  : _map(map) {
}

bool Apsis::Agent::Impeders::MapCollider::update(Apsis::Geometry::Rectangle& from,
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
      tileRect.x = x * 32.0;
      tileRect.y = y * 32.0;

      tileRect.width  = 32.0;
      tileRect.height = 32.0;

      for (int i = 0; i < 4; i++) {
        double tMin, tMax;
        Apsis::Geometry::Line l = vectors[i];
        if (tileRect.clip(&l, &tMin, &tMax)) {
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
          else if (tMax < t) {
            // New t value is the amount we will walk down the vector
            t = tMax;

            calculatedPoint = l.points[1];
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
        else {
          tMin = tMin;
          tMax = tMax;
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
