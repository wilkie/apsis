#include "apsis/rules/slide_map_collider.h"

#include "apsis/rules/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/engine/input.h"

#include "apsis/registry/state.h"

#include "apsis/rules/map_collider.h"

#include <math.h>

bool Apsis::Rules::SlideMapCollider::collide(const Apsis::World::Scene& scene,
                                                  const unsigned int objectId,
                                                  const Apsis::World::Object& object,
                                                  const Apsis::Geometry::Rectangle& original,
                                                  const Apsis::Geometry::Point& intended,
                                                  Apsis::World::CollisionObject& collidedWith,
                                                  Apsis::Geometry::Point& clipped) {
  if (Apsis::Rules::MapCollider::collide(scene,
                                                   objectId,
                                                   object,
                                                   original,
                                                   intended,
                                                   collidedWith,
                                                   clipped)) {
    // Adjust vector to remove collision direction and try again.
    if (collidedWith.type() == Apsis::World::CollisionObject::Type::Tile) {
      const Apsis::World::Tile& tile = collidedWith.tile();
      const Apsis::Geometry::Line& edge = collidedWith.edge();

      Apsis::Geometry::Point new_intended = intended;
      Apsis::Geometry::Rectangle new_original = original;

      Apsis::Geometry::Point clipped_1;
      Apsis::Geometry::Point clipped_2;

      new_original.x = (float)clipped.x;
      new_original.y = (float)clipped.y;

      // Try going horizontally
      new_intended.y = clipped.y;

      Apsis::World::CollisionObject collidedWith_1 = collidedWith;
      Apsis::World::CollisionObject collidedWith_2 = collidedWith;

      int winner = 1;
      float winner_distance = 0.0f;

      // Try collision again!
      if (!Apsis::Rules::MapCollider::collide(scene,
                                                        objectId,
                                                        object,
                                                        new_original,
                                                        new_intended,
                                                        collidedWith_1,
                                                        clipped_1)) {
        clipped_1 = new_intended;
      }

      Apsis::Geometry::Line line;
      line.points[0] = new_original.center();
      line.points[1] = clipped_1;
      winner_distance = (float)line.magnitude();

      // Try going vertically
      new_intended.y = intended.y;
      new_intended.x = clipped.x;

      // Try collision again!
      if (!Apsis::Rules::MapCollider::collide(scene,
                                                        objectId,
                                                        object,
                                                        new_original,
                                                        new_intended,
                                                        collidedWith_2,
                                                        clipped_2)) {
        clipped_2 = new_intended;
      }

      line.points[1] = clipped_2;
      if ((float)line.magnitude() > winner_distance) {
        winner = 2;
      }

      if (winner == 1) {
        collidedWith = collidedWith_1;
        clipped = clipped_1;
      }
      else {
        collidedWith = collidedWith_2;
        clipped = clipped_2;
      }
    }
    return true;
  }
  return false;
}