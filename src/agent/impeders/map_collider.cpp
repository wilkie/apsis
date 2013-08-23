#include "apsis/agent/impeders/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/engine/input.h"

#include "apsis/registry/state.h"

#include <math.h>

bool Apsis::Agent::Impeders::MapCollider::collide(const Apsis::World::Scene& scene,
                                                  const unsigned int objectId,
                                                  const Apsis::World::Object& object,
                                                  const Apsis::Geometry::Rectangle& original,
                                                  const Apsis::Geometry::Point& intended,
                                                  unsigned int collidedWith,
                                                  Apsis::Geometry::Point& clipped) {
  // For every point px, py in the set of 4 defined by given Rectangle

  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  double halfWidth  = ceil(original.width  / 2.0);
  double halfHeight = ceil(original.height / 2.0);

  points[0].x = original.x - halfWidth;
  points[0].y = original.y - halfHeight;
  toPoints[0].x = intended.x - halfWidth;
  toPoints[0].y = intended.y - halfHeight;

  points[1].x = original.x + halfWidth;
  points[1].y = original.y - halfHeight;
  toPoints[1].x = intended.x + halfWidth;
  toPoints[1].y = intended.y - halfHeight;

  points[2].x = original.x + halfWidth;
  points[2].y = original.y + halfHeight;
  toPoints[2].x = intended.x + halfWidth;
  toPoints[2].y = intended.y + halfHeight;

  points[3].x = original.x - halfWidth;
  points[3].y = original.y + halfHeight;
  toPoints[3].x = intended.x - halfWidth;
  toPoints[3].y = intended.y + halfHeight;

  Apsis::Geometry::Line vectors[4];
  for (int i = 0; i < 4; i++) {
    vectors[i].points[0] = points[i];
    vectors[i].points[1] = toPoints[i];
  }

  // Assume we can move the entire way
  double t = 1.0;

  Apsis::Geometry::Point calculatedPoint;

  // determine if there is an intersection with the map
  // TODO: optimizations
  for (unsigned int x = 0; x < scene.map(0).width(); x++) {
    for (unsigned int y = 0; y < scene.map(0).height(); y++) {
      const Apsis::World::Tile& tile = scene.map(0).tile(x, y);
      if (tile.passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Rectangle tileRect;
      tileRect.x = (x * scene.map(0).tileWidth()) + (scene.map(0).tileWidth() / 2.0f);
      tileRect.y = (y * scene.map(0).tileHeight()) + (scene.map(0).tileHeight() / 2.0f);

      tileRect.width  = scene.map(0).tileWidth();
      tileRect.height = scene.map(0).tileHeight();

      for (int i = 0; i < 4; i++) {
        double tMin, tMax;
        Apsis::Geometry::Line l = vectors[i];
        unsigned int edge = tileRect.clip(&l, &tMin, &tMax);
        if (edge > 0) {
          if (edge == 3) {
            if (!(tMin == 0.0 || tMax == 1.0)) {
              //object.enableState(_collideWithTopState);
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
    return false;
  }

  // Get the actual point we should move to
  clipped = calculatedPoint;
  return true;
}