#include "apsis/world/collision_object.h"

#include "apsis/registry/event.h"

#include "apsis/world/actor.h"
#include "apsis/world/tile.h"
#include "apsis/world/map.h"

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Actor& actor,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               CollisionType::CollisionTypes effect,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Actor),
    _collision_type(effect),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.actor = &actor;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Actor& actor,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               Apsis::Geometry::Line& redirection,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Actor),
    _collision_type(CollisionType::Redirected),
    _redirection(redirection),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.actor = &actor;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Map& map,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               CollisionType::CollisionTypes effect,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Map),
    _collision_type(effect),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.map = &map;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Map& map,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               Apsis::Geometry::Line& redirection,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Map),
    _collision_type(CollisionType::Redirected),
    _redirection(redirection),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.map = &map;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Tile& tile,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               CollisionType::CollisionTypes effect,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Tile),
    _collision_type(effect),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.tile = &tile;
}

Apsis::World::CollisionObject::CollisionObject(const Apsis::World::Tile& tile,
                                               const Apsis::Geometry::Line& edge,
                                               const Apsis::Geometry::Point& point,
                                               const Apsis::Geometry::Point& repositionPoint,
                                               Apsis::Geometry::Line& redirection,
                                               float direction)
  : _direction(direction),
    _object_type(ObjectType::Tile),
    _collision_type(CollisionType::Redirected),
    _redirection(redirection),
    _edge(edge),
    _repositionPoint(repositionPoint),
    _point(point) {
  _payload.tile = &tile;
}

Apsis::World::CollisionObject::ObjectType::ObjectTypes
Apsis::World::CollisionObject::objectType() const {
  return _object_type;
}

Apsis::World::CollisionObject::CollisionType::CollisionTypes
Apsis::World::CollisionObject::collisionType() const {
  return _collision_type;
}

const Apsis::World::Actor& Apsis::World::CollisionObject::actor() const {
  if (_object_type != ObjectType::Actor) {
    throw "CollisionObject::actor() : Object is not an Actor";
  }

  return *_payload.actor;
}

const Apsis::World::Tile& Apsis::World::CollisionObject::tile() const {
  if (_object_type != ObjectType::Tile) {
    throw "CollisionObject::actor() : Object is not a Tile";
  }

  return *_payload.tile;
}

const Apsis::World::Map& Apsis::World::CollisionObject::map() const {
  if (_object_type != ObjectType::Map) {
    throw "CollisionObject::actor() : Object is not a Map";
 } 

  return *_payload.map;
}

void Apsis::World::CollisionObject::actor(const Apsis::World::Actor& value,
                                          const Apsis::Geometry::Line& edge,
                                          const Apsis::Geometry::Point& point,
                                          CollisionType::CollisionTypes effect,
                                          float direction) {
  _object_type = ObjectType::Actor;
  _collision_type = effect;
  _payload.actor = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

void Apsis::World::CollisionObject::map(const Apsis::World::Map& value,
                                        const Apsis::Geometry::Line& edge,
                                        const Apsis::Geometry::Point& point,
                                        CollisionType::CollisionTypes effect,
                                        float direction) {
  _object_type = ObjectType::Map;
  _collision_type = effect;
  _payload.map = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

void Apsis::World::CollisionObject::tile(const Apsis::World::Tile& value,
                                         const Apsis::Geometry::Line& edge,
                                         const Apsis::Geometry::Point& point,
                                         CollisionType::CollisionTypes effect,
                                         float direction) {
  _object_type = ObjectType::Tile;
  _collision_type = effect;
  _payload.tile = &value;
  _direction = direction;
  _point = point;
  _edge = edge;
}

float Apsis::World::CollisionObject::direction() const {
  return _direction;
}

const Apsis::Geometry::Line& Apsis::World::CollisionObject::redirection() const {
  return _redirection;
}

const Apsis::Geometry::Point& Apsis::World::CollisionObject::point() const {
  return _point;
}

const Apsis::Geometry::Point& Apsis::World::CollisionObject::repositionPoint() const {
  return _repositionPoint;
}

const Apsis::Geometry::Line& Apsis::World::CollisionObject::edge() const {
  return _edge;
}

unsigned int Apsis::World::CollisionObject::collideEvent() const {
  std::string str = "collide_with_";

  switch (_object_type) {
  case ObjectType::Tile:
    str = str.append("tile");
    break;
  case ObjectType::Actor:
    str = str.append("actor_");
    str = str.append(_payload.actor->name());
    break;
  case ObjectType::Map:
    str = str.append("map");
    break;
  }

  return Apsis::Registry::Event::id(str.c_str());
}