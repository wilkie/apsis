#include "apsis/world/actor.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include "apsis/agent/movers/or.h"

#include <fstream>
#include <json/json.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::World::Actor::~Actor() {
}

Apsis::World::Actor::Actor(const char* actorFile,
                           unsigned int x,
                           unsigned int y) {
  _currentAnimation = NULL;
  _currentFrame = 0;
  _currentTime = 0;

  _position.x = (float)x;
  _position.y = (float)y;

  _parseJSONFile(actorFile);

  if (_animations.size() > 0) {
    _currentAnimation = &_animations[0];
    _frame = &_currentAnimation->frame(0);
  }
}

char* Apsis::World::Actor::_newState(const char* name) {
  char* ret = new char[129];
  strncpy(ret, name, 128);
  _states.push_back(ret);
  return ret;
}

void Apsis::World::Actor::state(const char* stateName) {
  for (unsigned int i = 0; i < _states.size(); i++) {
    if (strncmp(_states[i], stateName, 128) == 0) {
      _state = _states[i];
    }
  }
}

const char* Apsis::World::Actor::state() {
  return _state;
}

Apsis::Sprite::Sheet* Apsis::World::Actor::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::World::Actor::position() {
  return _position;
}

void Apsis::World::Actor::animate(const char* animationName) {
  // Look-up the animation
  for (unsigned int i = 0; i < _animations.size(); i++) {
    if (_animations[i].name() == std::string(animationName)) {
      _currentAnimation = &_animations[i];
      _currentFrame = 0;
      _frame = &_currentAnimation->frame(_currentFrame);
      return;
    }
  }
}

void Apsis::World::Actor::nextFrame() {
  _currentFrame += 1;
  if (_currentAnimation->count() == 0) {
    _currentFrame = 0;
  }
  else {
    _currentFrame %= _currentAnimation->count();
  }
  _frame = &_currentAnimation->frame(_currentFrame);
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

void Apsis::World::Actor::draw(glm::mat4& projection,
                               Primitives::Camera& camera) {
  glm::mat4 model = glm::translate(glm::mat4(1.0),
                                   glm::vec3(_position.x, 0.0, _position.y));

  _spriteSheet->draw(_frame->spriteIndex, projection, camera, model);
}

char* Apsis::World::Actor::rules() {
  char* ret = new char[1024];

  strcpy(ret, "");

  for (unsigned int i = 0; i < _moverAgents.size(); i++) {
    strcat(ret, _moverAgents[i]->rule());
    strcat(ret, "\n");
  }

  for (unsigned int i = 0; i < _impederAgents.size(); i++) {
    strcat(ret, _impederAgents[i]->rule());
    strcat(ret, "\n");
  }

  return ret;
}

void Apsis::World::Actor::_parseJSONFile(const char* filename) {
  Json::Reader reader;
  Json::Value value;

  std::ifstream file(filename);
  reader.parse(file, value);
  file.close();

  _position.width  = (float)value["width"].asDouble();
  _position.height = (float)value["height"].asDouble();

  // TODO: account for null sprites value
  _spriteSheet = new Apsis::Sprite::Sheet(value["sprites"].asCString());

  // Animation
  // TODO: better handling of invalid values
  for (Json::Value::iterator it = value["animations"].begin(); it != value["animations"].end(); it++) {
    // Create an animation structure
    Sprite::Animation newAnimation((*it)["name"].asCString());

    // Store all of the sprites
    int spriteIndex = -1;
    do {
      spriteIndex = _spriteSheet->enumerateSprites((*it)["sprites"].asCString(), spriteIndex+1);
      if (spriteIndex != -1) {
        newAnimation.addFrame(spriteIndex);
      }
    } while(spriteIndex != -1);

    _animations.push_back(newAnimation);
  }
}