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
      struct Type {
        enum Types {
          Actor,
          Map,
          Tile,
        };
      };

      /*
       *  Creates a collision object that represents an Actor from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Actor& actor,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      float direction);

      /*
       *  Creates a collision object that represents an Map from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Map& map,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      float direction);

      /*
       *  Creates a collision object that represents an Tile from the given
       *  direction.
       */
      CollisionObject(const Apsis::World::Tile& tile,
                      const Apsis::Geometry::Line& edge,
                      const Apsis::Geometry::Point& point,
                      float direction);

      /*
       *  Returns the type of collision object.
       */
      Type::Types type() const;

      /*
       *  Returns the direction the colliding object is going to hit this
       *  object.
       */
      float direction() const;

      /*
       *  Returns the edge that was hit upon the collision.
       */
      const Apsis::Geometry::Line& edge() const;

      /*
       *  Returns the point that was hit upon the collision.
       */
      const Apsis::Geometry::Point& point() const;

      /*
       *  Returns the Actor object that resulted in this collision.
       */
      const Apsis::World::Actor& actor() const;

      /*
       *  Sets this collision object to reflect the given Actor.
       */
      void actor(const Apsis::World::Actor& value,
                 float direction,
                 const Apsis::Geometry::Line& edge,
                 const Apsis::Geometry::Point& point);

      /*
       *  Returns the Tile object that resulted in this collision.
       */
      const Apsis::World::Tile& tile() const;

      /*
       *  Sets this collision object to reflect the given Tile.
       */
      void tile(const Apsis::World::Tile& value,
                float direction,
                const Apsis::Geometry::Line& edge,
                const Apsis::Geometry::Point& point);

      /*
       *  Returns the Map object that resulted in this collision.
       */
      const Apsis::World::Map& map() const;

      /*
       *  Sets this collision object to reflect the given Map.
       */
      void map(const Apsis::World::Map& value,
               float direction,
               const Apsis::Geometry::Line& edge,
               const Apsis::Geometry::Point& point);

      /*
       *  Returns the collided_with_* event that corresponds to this
       *  object.
       */
      unsigned int collideEvent() const;

    private:
      // Direction that the object is moving.
      float _direction;

      // Type of object
      Type::Types _type;

      // Position
      Apsis::Geometry::Line  _edge;
      Apsis::Geometry::Point _point;

      union {
        const Apsis::World::Actor* actor;
        const Apsis::World::Map*   map;
        const Apsis::World::Tile*  tile;
      } _payload;
    };
  }
}

#endif