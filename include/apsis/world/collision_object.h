#ifndef APSIS_WORLD_COLLISION_OBJECT_H
#define APSIS_WORLD_COLLISION_OBJECT_H

#include "apsis/geometry/line.h"
#include "apsis/geometry/point.h"

namespace Apsis {
  namespace World {
    class Actor;
    class Map;
    class Tile;

    class CollisionObject {
    public:
      struct ObjectType {
        enum ObjectTypes {
          Actor,
          Map,
          Tile,
        };
      };

      struct CollisionType {
        enum CollisionTypes {
          /*
           *  Indicates that the moving object passed through the collided
           *  object.
           */
          Passed,

          /*
           *  Indicates that the moving object is stopped by the collided
           *  object.
           */
          Impeded,

          /*
           *  Indicates that the moving object has been redirected by the
           *  collided object and is still moving.
           */
          Redirected,
        };
      };

      /*
       *  Creates a collision object that represents an Actor from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Actor& actor,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      CollisionType::CollisionTypes effect,
                      float direction);

      /*
       *  Creates a collision object that represents an Actor from the given
       *  direction that redirects in the given line.
       */
      CollisionObject(const Apsis::World::Actor& actor,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      Apsis::Geometry::Line& redirection,
                      float direction);

      /*
       *  Creates a collision object that represents an Map from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Map& map,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      CollisionType::CollisionTypes effect,
                      float direction);

      /*
       *  Creates a collision object that represents an Map from the given
       *  direction that redirects in the given line.
       */
      CollisionObject(const Apsis::World::Map& map,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      Apsis::Geometry::Line& redirection,
                      float direction);

      /*
       *  Creates a collision object that represents an Tile from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Tile& tile,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      CollisionType::CollisionTypes effect,
                      float direction);

      /*
       *  Creates a collision object that represents an Tile from the given
       *  direction that redirects in the given line.
       */
      CollisionObject(const Apsis::World::Tile& tile,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      const Apsis::Geometry::Point& repositionPoint,
                      Apsis::Geometry::Line& redirection,
                      float direction);

      /*
       *  Returns the type of collision object.
       */
      ObjectType::ObjectTypes objectType() const;

      /*
       *  Returns the type of collision that occured.
       */
      CollisionType::CollisionTypes collisionType() const;

      /*
       *  Returns the direction the colliding object is going to hit this
       *  object.
       */
      float direction() const;

      /*
       *  Returns the redirection vector that should be followed as a result
       *  of this collision.
       */
      const Apsis::Geometry::Line& redirection() const;

      /*
       *  Returns the edge that was hit upon the collision.
       */
      const Apsis::Geometry::Line& edge() const;

      /*
       *  Returns the point that was hit upon the collision.
       */
      const Apsis::Geometry::Point& point() const;

      /*
       *  Returns the point the object that hit should be positioned in order
       *  to rest touching the collision object.
       */
      const Apsis::Geometry::Point& repositionPoint() const;

      /*
       *  Returns the Actor object that resulted in this collision.
       */
      const Apsis::World::Actor& actor() const;

      /*
       *  Sets this collision object to reflect the given Actor.
       */
      void actor(const Apsis::World::Actor& value,
                 const Apsis::Geometry::Line& edge,
                 const Apsis::Geometry::Point& point,
                 CollisionType::CollisionTypes effect,
                 float direction);

      /*
       *  Returns the Tile object that resulted in this collision.
       */
      const Apsis::World::Tile& tile() const;

      /*
       *  Sets this collision object to reflect the given Tile.
       */
      void tile(const Apsis::World::Tile& value,
                const Apsis::Geometry::Line& edge,
                const Apsis::Geometry::Point& point,
                CollisionType::CollisionTypes effect,
                float direction);

      /*
       *  Returns the Map object that resulted in this collision.
       */
      const Apsis::World::Map& map() const;

      /*
       *  Sets this collision object to reflect the given Map.
       */
      void map(const Apsis::World::Map& value,
               const Apsis::Geometry::Line& edge,
               const Apsis::Geometry::Point& point,
               CollisionType::CollisionTypes effect,
               float direction);

      /*
       *  Returns the collided_with_* event that corresponds to this
       *  object.
       */
      unsigned int collideEvent() const;

    private:
      // Direction that the object is moving.
      float _direction;

      // Type of object
      ObjectType::ObjectTypes _object_type;

      // Type of collision
      CollisionType::CollisionTypes _collision_type;

      // Redirection Line
      Apsis::Geometry::Line _redirection;

      // Position
      Apsis::Geometry::Line  _edge;
      Apsis::Geometry::Point _point;

      // Reposition Point
      Apsis::Geometry::Point _repositionPoint;

      union {
        const Apsis::World::Actor* actor;
        const Apsis::World::Map*   map;
        const Apsis::World::Tile*  tile;
      } _payload;
    };
  }
}

#endif