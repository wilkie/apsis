#include "apsis/rules/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/engine/input.h"

#include "apsis/registry/state.h"

#include <math.h>

bool Apsis::Rules::MapCollider::collide(const Apsis::World::Scene& scene,
                                        const unsigned int objectId,
                                        const Apsis::World::Object& object,
                                        const Apsis::Geometry::Rectangle& original,
                                        const Apsis::Geometry::Point& intended,
                                        Apsis::World::CollisionObject& collidedWith,
                                        Apsis::Geometry::Point& clipped) {
  // For every point px, py in the set of 4 defined by given Rectangle

  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  double halfWidth  = ceil(original.width  / 2.0);
  double halfHeight = ceil(original.height / 2.0);

  original.points(points);

  toPoints[0].x = intended.x - halfWidth;
  toPoints[0].y = intended.y - halfHeight;

  toPoints[1].x = intended.x + halfWidth;
  toPoints[1].y = intended.y - halfHeight;

  toPoints[2].x = intended.x + halfWidth;
  toPoints[2].y = intended.y + halfHeight;

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
          if (tMax < 0.00005) { continue; }
          if (tMin < t) {
            // New t value is the amount we will walk down the vector
            t = tMin;

            const Apsis::Geometry::Point& point = l.points[0];

            calculatedPoint = point;
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

            if (tMin < 0.00005 && tMax > 0.99995) {
              // Inside the rectangle... Usually this means it hit a corner
              // TODO: figure out which edge should be considered as blocking
              //       1. determine the corner
              //       2. determine if one edge is obscured by another tile
              //          2a. if so, the other edge is the blocking edge
              //          2b. otherwise, that edge is the blocking edge
              //       (it will be biased toward the compared edge)

              edge = 0;

              if (point.x - 0.00005 < tileRect.x - (tileRect.width / 2.0f) &&
                  point.x + 0.00005 > tileRect.x - (tileRect.width / 2.0f)) {
                // Check Left Edge for a tile that obscures it
                // If it is not obscured, mark it as the edge
                if (x != 0) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x-1, y);
                  if (check.passable()) {
                    edge = 1;
                  }
                }
              }
              else {
                // Check Right Edge for a tile that obscures it
                // If it is not obscured, mark it as the edge
                if (x != scene.map(0).width()-1) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x+1, y);
                  if (check.passable()) {
                    edge = 2;
                  }
                }
              }

              if (edge == 0) {
                if (point.y - 0.00005 < tileRect.y - (tileRect.width / 2.0f) &&
                    point.y + 0.00005 > tileRect.y - (tileRect.width / 2.0f)) {
                  edge = 3;
                }
                else {
                  edge = 4;
                }
              }
            }
            else if (tMax < 0.00005) {
              tMax = tMax;
            }

            collidedWith.tile(tile, 0.0f, tileRect.edge(edge - 1), l.points[0]);
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