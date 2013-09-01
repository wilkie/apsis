#include "apsis/world/collision_object.h"

#include "apsis/registry/event.h"

#include "apsis/world/actor.h"
#include "apsis/world/tile.h"
#include "apsis/world/map.h"

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Actor& actor,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               float direction)
  : _direction(direction),
    _type(Apsis::World::CollisionObject::Type::Actor),
    _edge(edge),
    _point(point) {
  _payload.actor = &actor;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Map& map,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               float direction)
  : _direction(direction),
    _type(Apsis::World::CollisionObject::Type::Map),
    _edge(edge),
    _point(point) {
  _payload.map = &map;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Tile& tile,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               float direction)
  : _direction(direction),
    _type(Apsis::World::CollisionObject::Type::Tile),
    _edge(edge),
    _point(point) {
  _payload.tile = &tile;
}

Apsis::World::CollisionObject::Type::Types Apsis::World::CollisionObject::type() const {
  return _type;
}

const Apsis::World::Actor& Apsis::World::CollisionObject::actor() const {
  if (_type != Type::Actor) {
    throw "CollisionObject::actor() : Object is not an Actor";
  }

  return *_payload.actor;
}

const Apsis::World::Tile& Apsis::World::CollisionObject::tile() const {
  if (_type != Type::Tile) {
    throw "CollisionObject::actor() : Object is not a Tile";
  }

  return *_payload.tile;
}

const Apsis::World::Map& Apsis::World::CollisionObject::map() const {
  if (_type != Type::Map) {
    throw "CollisionObject::actor() : Object is not a Map";
 } 

  return *_payload.map;
}

void Apsis::World::CollisionObject::actor(const Apsis::World::Actor& value,
                                          float direction,
                                          const Apsis::Geometry::Line& edge,
                                          const Apsis::Geometry::Point& point) {
  _type = Type::Actor;
  _payload.actor = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

void Apsis::World::CollisionObject::map(const Apsis::World::Map& value,
                                        float direction,
                                        const Apsis::Geometry::Line& edge,
                                        const Apsis::Geometry::Point& point) {
  _type = Type::Map;
  _payload.map = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

void Apsis::World::CollisionObject::tile(const Apsis::World::Tile& value,
                                         float direction,
                                         const Apsis::Geometry::Line& edge,
                                         const Apsis::Geometry::Point& point) {
  _type = Type::Tile;
  _payload.tile = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

float Apsis::World::CollisionObject::direction() const {
  return _direction;
}

const Apsis::Geometry::Point& Apsis::World::CollisionObject::point() const {
  return _point;
}

const Apsis::Geometry::Line& Apsis::World::CollisionObject::edge() const {
  return _edge;
}

unsigned int Apsis::World::CollisionObject::collideEvent() const {
  std::string str = "collided_with_";

  switch (_type) {
  case Type::Tile:
    str = str.append("tile");
    break;
  case Type::Actor:
    str = str.append(_payload.actor->name());
    break;
  case Type::Map:
    str = str.append("map");
    break;
  }

  return Apsis::Registry::Event::id(str.c_str());
}