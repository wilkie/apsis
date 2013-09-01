#include "apsis/world/actor.h"

#include "apsis/world/collision_object.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include <fstream>
#include <string>
#include <json/json.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::World::Actor::Actor(const Apsis::Sprite::Thing& thing,
                           unsigned int x,
                           unsigned int y) 
  : _thing(thing),
    _sheet(thing.sheet()) {

  // Copy over object properties
  _object = thing.object();

  const Apsis::Sprite::Animation& animation = _thing.animationById(0);
  _animation = &animation;
  _frame     = &_animation->frame(0);

  _currentFrame = 0;
  _currentTime = 0;

  const Apsis::World::RuleSet& rules = thing.rules();
  for (unsigned int i = 0; i < rules.count(); i++) {
    _ruleSet.addRule(rules.rule(i));
  }

  _position.x = (float)x;
  _position.y = (float)y;
  _position.width  = (float)_object.get("width").asDouble();
  _position.height = (float)_object.get("height").asDouble();
}

const Apsis::Sprite::Sheet& Apsis::World::Actor::sheet() const {
  return _sheet;
}

Apsis::Geometry::Rectangle Apsis::World::Actor::position() const {
  return _position;
}

void Apsis::World::Actor::animate(const char* animationName) {
  _animation = &_thing.animation(animationName);
}

void Apsis::World::Actor::nextFrame() {
  _currentFrame += 1;
  if (_animation->count() == 0) {
    _currentFrame = 0;
  }
  else {
    _currentFrame %= _animation->count();
  }
  _frame = &_animation->frame(_currentFrame);
  _currentTime = 0;
}

void Apsis::World::Actor::attachRule(const Apsis::Registry::Rule& rule) {
  _ruleSet.addRule(rule);
}

void Apsis::World::Actor::act(Apsis::World::Scene& scene, unsigned int action_id, bool held) {
  _ruleSet.act(action_id, held, _object, scene);
}

void Apsis::World::Actor::update(Apsis::World::Scene& scene, float elapsed) {
  _currentTime += elapsed;
  if (_currentTime > 0.08f) {
    nextFrame();
  }

  _object.set("x", _position.x);
  _object.set("y", _position.y);

  _ruleSet.update(elapsed, _object, scene);
}

void Apsis::World::Actor::collide(Apsis::World::Scene& scene) {
  Apsis::Geometry::Line  line;
  Apsis::Geometry::Point point;
  Apsis::World::CollisionObject collidedWith(*this, line, point, 0.0f);

  Apsis::Geometry::Point to;
  Apsis::Geometry::Point clipped;

  to.x = (float)_object.get("x").asDouble();
  to.y = (float)_object.get("y").asDouble();

  if (to.x == _position.x && to.y == _position.y) {
    return;
  }

  clipped = to;

  if (_ruleSet.collide(scene, _object, _position, to, collidedWith, clipped)) {
    to = clipped;

    unsigned int event_id = collidedWith.collideEvent();

    if (_object.respondsTo(event_id)) {
      _object.enqueueEvent(event_id);
    }
  }

  _position.x = (float)to.x;
  _position.y = (float)to.y;
}

void Apsis::World::Actor::respond(Apsis::World::Scene& scene) {
  while (_object.hasEvents()) {
    unsigned int event_id = _object.dequeueEvent();
  }
}

void Apsis::World::Actor::draw(const glm::mat4& projection,
                               const Primitives::Camera& camera) const {
  glm::mat4 model = glm::translate(glm::mat4(1.0),
                                   glm::vec3(_position.x, 0.0, _position.y));

  _sheet.draw(_frame->spriteIndex, projection, camera, model);
}

const char* Apsis::World::Actor::name() const {
  return _thing.name();
}