#include "apsis/rules/actor_collider.h"

bool Apsis::Rules::ActorCollider::collide(const Apsis::World::Scene& scene,
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

  Apsis::Geometry::Point calculatedPoint;

  // determine if there is an intersection with the map
  // TODO: Multiple maps
  // TODO: Map data is already on the gpu, so we could do all collisions
  //       on the gpu too?
  for (unsigned int i = 0; i < scene.actorCount(); i++) {
    const Apsis::World::Actor& actor = scene.actor(i);

    if (actor.object().isMe(object)) {
      // Do not check collisions with self.
      continue;
    }

    // Do we intersect?
    Apsis::Geometry::Rectangle& rect = actor.position();

    for (int i = 0; i < 4; i++) {
      float tMin, tMax;
      Apsis::Geometry::Line l = vectors[i];
      unsigned int edge = rect.clip(&l, &tMin, &tMax);
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

          if (tMax < 0.00005) {
            tMax = tMax;
          }

          Apsis::World::CollisionObject::CollisionType::CollisionTypes collision_type;

          static unsigned int passable_id = Apsis::Registry::Property::id("passable");

          if (!actor.object().has(passable_id) ||
              actor.object().get(passable_id).asInteger() != 0) {
            collision_type = Apsis::World::CollisionObject::CollisionType::Passed;
          }
          else {
            collision_type = Apsis::World::CollisionObject::CollisionType::Impeded;
          }

          object.addCollision(t, Apsis::World::CollisionObject(actor,
                                                               actor.position().edge(edge - 1),
                                                               l.points[0],
                                                               calculatedPoint,
                                                               collision_type,
                                                               0.0f));
        }
      }
    }
  }

  if (t == 1.0) {
    return false;
  }

  return true;
}