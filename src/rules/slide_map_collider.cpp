#include "apsis/rules/slide_map_collider.h"

#include "apsis/rules/map_collider.h"

#include "apsis/geometry/point.h"

#include "apsis/engine/input.h"

#include "apsis/registry/state.h"

#include "apsis/rules/map_collider.h"

bool Apsis::Rules::SlideMapCollider::collide(const Apsis::World::Scene& scene,
                                             Apsis::World::Object& object,
                                             const Apsis::Geometry::Rectangle& original,
                                             const Apsis::Geometry::Point& intended) {
  // Use MapCollider as a base
  if (Apsis::Rules::MapCollider::collide(scene,
                                         object,
                                         original,
                                         intended)) {
    // Determine redirection vector
    for (unsigned int i = 0; i < object.collisionCount(); i++) {
      const Apsis::World::CollisionObject& collisionObject = object.collisionObject(i);

      if (collisionObject.collisionType() == Apsis::World::CollisionObject::CollisionType::Impeded) {
        if (collisionObject.objectType() == Apsis::World::CollisionObject::ObjectType::Tile) {
          float t = object.collisionPeriod(i);

          Apsis::Geometry::Line trajectory;
          trajectory.points[0] = original.center();
          trajectory.points[1] = intended;
          // This would be the first impeding tile... set a redirect vector on this collision object

          const Apsis::World::Tile& tile = collisionObject.tile();
          const Apsis::Geometry::Line& edge = collisionObject.edge();

          // Continue our trajectory. We will alter it to glide against the map edge.
          Apsis::Geometry::Line redirect;
          redirect.points[0] = collisionObject.repositionPoint();
          redirect.points[1] = intended;

          if (edge.points[0].x == edge.points[1].x) {
            // Hit left or right edge, redirect vertically
            redirect.points[1].x = redirect.points[0].x;
          }
          else {
            // Hit top or bottom edge, redirect horizontally
            redirect.points[1].y = redirect.points[0].y;
          }

          if (!redirect.equals(trajectory)) {
            // Reset the collision object to now contain a redirection vector.
            object.addCollision(t, Apsis::World::CollisionObject(tile,
                                                                 edge,
                                                                 collisionObject.point(),
                                                                 collisionObject.repositionPoint(),
                                                                 redirect,
                                                                 collisionObject.direction()));
          }
        }

        // Regardless, we are impeded. So stop going.
        break;
      }
    }
    return true;
  }

  return false;
}