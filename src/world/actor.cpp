#include "apsis/world/actor.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include "apsis/agent/movers/or.h"

#include <fstream>
#include <string>
#include <json/json.h>

#include <Windows.h>

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

void Apsis::World::Actor::attachMover(Apsis::Agent::Mover* agent) {
  for (unsigned int i = 0; i < _moverAgents.size(); i++) {
    if (_moverAgents[i]->supercedes(agent->rule())) {
      Apsis::Agent::Mover* mover = new Apsis::Agent::Movers::Or(_moverAgents[i], agent);
      _moverAgents.erase(_moverAgents.begin() + i);
      attachMover(mover);
      return;
    }

    if (agent->supercedes(_moverAgents[i]->rule())) {
      Apsis::Agent::Mover* mover = new Apsis::Agent::Movers::Or(agent, _moverAgents[i]);
      _moverAgents.erase(_moverAgents.begin() + i);
      attachMover(mover);
      return;
    }
  }

  _moverAgents.push_back(agent);
}

void Apsis::World::Actor::attachImpeder(Apsis::Agent::Impeder* agent) {
  _impederAgents.push_back(agent);
}

void Apsis::World::Actor::update(float elapsed) {
  _currentTime += elapsed;
  if (_currentTime > 0.08f) {
    nextFrame();
  }

  Apsis::Geometry::Point to;
  to.x = _position.x;
  to.y = _position.y;

  for (unsigned int i = 0; i < _moverAgents.size(); i++) {
    if (_moverAgents[i]->update(elapsed, _object, _position, to)) {
      this->move(to);
    }
  }
}

void Apsis::World::Actor::move(Apsis::Geometry::Point& to) {
  for (unsigned int i = 0; i < _impederAgents.size(); i++) {
    _impederAgents[i]->update(_object, _position, to);
  }

  _position.x = (float)to.x;
  _position.y = (float)to.y;
}

void Apsis::World::Actor::draw(const glm::mat4& projection,
                               Primitives::Camera& camera) const {
  glm::mat4 model = glm::translate(glm::mat4(1.0),
                                   glm::vec3(_position.x, 0.0, _position.y));

  _sheet.draw(_frame->spriteIndex, projection, camera, model);
}

const char* Apsis::World::Actor::rules() const {
  std::string ret = std::string("");

  for (unsigned int i = 0; i < _moverAgents.size(); i++) {
    ret.append(_moverAgents[i]->rule());
  }

  for (unsigned int i = 0; i < _impederAgents.size(); i++) {
    ret.append(_impederAgents[i]->rule());
    ret.append("\n");
  }

  return ret.c_str();
}