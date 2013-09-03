#include "apsis/rules/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/engine/input.h"

#include "apsis/registry/state.h"

#include <math.h>

bool Apsis::Rules::MapCollider::collide(const Apsis::World::Scene& scene,
                                        Apsis::World::Object& object,
                                        const Apsis::Geometry::Rectangle& original,
                                        const Apsis::Geometry::Point& intended) {
  // For every point px, py in the set of 4 defined by given Rectangle

  Apsis::Geometry::Point points[4];
  Apsis::Geometry::Point toPoints[4];

  float halfWidth  = ceil(original.width  / 2.0f);
  float halfHeight = ceil(original.height / 2.0f);

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
  float t = 1.0;

  // determine if there is an intersection with the map
  // TODO: Multiple maps
  // TODO: Map data is already on the gpu, so we could do all collisions
  //       on the gpu too?
  const Apsis::World::Map& map = scene.map(0);

  unsigned int min_x, min_y, max_x, max_y;
  min_x = 0;
  max_x = map.width();

  min_y = 0;
  max_y = map.height();

  unsigned int tmp;

  tmp = (unsigned int)(points[0].x / (unsigned int)map.tileWidth()) - 1;

  if (tmp > min_x) {
    min_x = tmp;
  }

  tmp = (unsigned int)(points[2].x / (unsigned int)map.tileWidth()) + 1;

  if (tmp < max_x) {
    max_x = tmp;
  }

  tmp = (unsigned int)(points[0].y / (unsigned int)map.tileHeight()) - 1;

  if (tmp > min_y) {
    min_y = tmp;
  }

  tmp = (unsigned int)(points[2].y / (unsigned int)map.tileHeight()) + 1;

  if (tmp < max_y) {
    max_y = tmp;
  }

  Apsis::Geometry::Point calculatedPoint = {0.0f, 0.0f};

  for (unsigned int x = min_x; x <= max_x; x++) {
    for (unsigned int y = min_y; y <= max_y; y++) {
      const Apsis::World::Tile& tile = map.tile(x, y);
      if (tile.passable()) {
        continue;
      }

      // Do we intersect?
      Apsis::Geometry::Rectangle tileRect;

      tileRect.width  = map.tileWidth();
      tileRect.height = map.tileHeight();

      tileRect.x = (x * map.tileWidth()) + (map.tileWidth() / 2.0f);
      tileRect.y = (y * map.tileHeight()) + (map.tileHeight() / 2.0f);

      for (int i = 0; i < 4; i++) {
        float tMin, tMax;
        Apsis::Geometry::Line l = vectors[i];
        if (l.points[1].y > 490.0f) {
          l = vectors[i];
        }
        unsigned int edge = tileRect.clip(&l, &tMin, &tMax);
        if (edge > 0) {
          if (tMin - 0.00005 <= t) {
            const Apsis::Geometry::Point& point = l.points[0];

            Apsis::Geometry::Point lastPoint = calculatedPoint;

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

            // Is this value the previous record? if so, consolidate!
            if (tMin + 0.00005 >= t) {
              // Hmm. We see another collision at the same place... We hit a corner.
              // Brace ourselves and consolidate based on the new edge.

              if (edge == 1 || edge == 2) { // Left/right edge
                calculatedPoint.y = lastPoint.y;
              }
              else { // Top/bottom edge
                calculatedPoint.x = lastPoint.x;
              }
            }

            // New t value is the amount we will walk down the vector
            t = tMin;

            if (point.x - 0.00005 < tileRect.x - (tileRect.width / 2.0f) &&
                point.x + 0.00005 > tileRect.x - (tileRect.width / 2.0f)) {  // Left edge

              // We aren't colliding if the intended line radiates away from this edge.
              if (intended.x + 0.00005 > calculatedPoint.x) {
                edge = 1;

                // Reject if edge is between two solid blocks
                if (x != 0) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x-1, y);
                  if (!check.passable()) {
                    edge = 0;
                  }
                }
              }
              else if (edge == 1) {
                edge = 0;
              }
            }
            else if (point.x - 0.00005 < tileRect.x + (tileRect.width / 2.0f) &&
                      point.x + 0.00005 > tileRect.x + (tileRect.width / 2.0f)) {  // Right edge

              // We aren't colliding if the intended line radiates away from this edge.
              if (intended.x - 0.00005 < calculatedPoint.x) {
                edge = 2;

                // Reject if edge is between two solid blocks
                if (x != scene.map(0).width()-1) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x+1, y);
                  if (!check.passable()) {
                    edge = 0;
                  }
                }
              }
              else if (edge == 2) {
                edge = 0;
              }
            }
            else if (point.y - 0.00005 < tileRect.y - (tileRect.height / 2.0f) &&
                     point.y + 0.00005 > tileRect.y - (tileRect.height / 2.0f)) { // Top edge

              // We aren't colliding if the intended line radiates away from this edge.
              if (intended.y + 0.00005 > calculatedPoint.y) {
                edge = 3;

                // Reject if edge is between two solid blocks
                if (y != 0) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x, y-1);
                  if (!check.passable()) {
                    edge = 0;
                  }
                }
              }
              else if (edge == 3) {
                edge = 0;
              }
            }
            else if (point.y - 0.00005 < tileRect.y + (tileRect.height / 2.0f) &&
                     point.y + 0.00005 > tileRect.y + (tileRect.height / 2.0f)) { // Bottom edge

              // We aren't colliding if the intended line radiates away from this edge.
              if (intended.y - 0.00005 < calculatedPoint.y) {
                edge = 4;

                // Reject if edge is between two solid blocks
                if (y != scene.map(0).height()-1) {
                  const Apsis::World::Tile& check = scene.map(0).tile(x, y+1);
                  if (!check.passable()) {
                    edge = 0;
                  }
                }
              }
              else if (edge == 4) {
                edge = 0;
              }
            }

            if (edge > 0 && t >= 0.0f && t <= 1.0f) {
              object.addCollision(t, Apsis::World::CollisionObject(tile,
                                                                   tileRect.edge(edge - 1),
                                                                   l.points[0],
                                                                   calculatedPoint,
                                                                   Apsis::World::CollisionObject::CollisionType::Impeded,
                                                                   0.0f));
            }
          }
        }
      }
    }
  }

  if (t == 1.0) {
    return false;
  }

  return true;
}