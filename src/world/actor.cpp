#include "apsis/world/actor.h"

Apsis::World::Actor::~Actor() {
  // Deallocate animations
  for (unsigned int i = 0; i < _animations.size(); i++) {
    for (unsigned int j = 0; j < _animations[i]->frames.size(); j++) {
      delete _animations[i]->frames[j];
    }
    delete _animations[i];
  }
}

Apsis::World::Actor::Actor(const char* actorFile,
                           unsigned int x,
                           unsigned int y) {
  _currentAnimation = NULL;
  _currentFrame = 0;
  _frame = NULL;
  _currentTime = 0;

  FILE* f = fopen(actorFile, "rt");

  _position.x = x;
  _position.y = y;

  char key[129];
  char val[129];

  while(!feof(f)) {
    fscanf(f, "%128s %128s\n", key, val);

    if (strcmp(key, "width") == 0) {
      _position.width = atoi(val);
    }
    else if (strcmp(key, "height") == 0) {
      _position.height = atoi(val);
    }
    else if (strcmp(key, "move_rate") == 0) {
      _moveRate = atoi(val);
    }
    else if (strcmp(key, "sprites") == 0) {
      _spriteSheet = new Apsis::Primitives::SpriteSheet(val);
    }
    else if (strcmp(key, "state") == 0) {
      _newState(val);
    }
    else if (strcmp(key, "default_state") == 0) {
      state(val);
    }
    else {
      // Animation

      // Create an animation structure
      Animation* newAnimation = _newAnimation(key);

      // Store all of the sprites
      int spriteIndex = -1;
      do {
        spriteIndex = _spriteSheet->enumerateSprites(val, spriteIndex+1);
        if (spriteIndex != -1) {
          AnimationFrame* frame = new AnimationFrame;
          _spriteSheet->textureCoordinates(spriteIndex, frame->textureCoordinates);
          frame->sprite = _spriteSheet->sprite(spriteIndex);
          newAnimation->frames.push_back(frame);
        }
      } while(spriteIndex != -1);
    }
  }
}

Apsis::World::Animation* Apsis::World::Actor::_newAnimation(const char* name) {
  Animation* ret = new Animation;
  strncpy(ret->name, name, 128);
  _animations.push_back(ret);
  return ret;
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

Apsis::Primitives::SpriteSheet* Apsis::World::Actor::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::World::Actor::position() {
  return _position;
}

void Apsis::World::Actor::animate(const char* animationName) {
  // Look-up the animation
  for (unsigned int i = 0; i < _animations.size(); i++) {
    if (strncmp(_animations[i]->name, animationName, 128) == 0) {
      _currentAnimation = _animations[i];
      _currentFrame = 0;
      _frame = _currentAnimation->frames[_currentFrame];
      return;
    }
  }
}

void Apsis::World::Actor::nextFrame() {
  _currentFrame += 1;
  if (_currentAnimation->frames.size() == 0) {
    _currentFrame = 0;
  }
  else {
    _currentFrame %= _currentAnimation->frames.size();
  }
  _frame = _currentAnimation->frames[_currentFrame];
  _currentTime = 0;
}

void Apsis::World::Actor::textureCoordinates(double coords[4]) {
  coords[0] = _frame->textureCoordinates[0];
  coords[1] = _frame->textureCoordinates[1];
  coords[2] = _frame->textureCoordinates[2];
  coords[3] = _frame->textureCoordinates[3];
}

Apsis::Primitives::Sprite* Apsis::World::Actor::sprite() {
  return _frame->sprite;
}

void Apsis::World::Actor::attachMover(Apsis::Agent::Mover& agent) {
  _moverAgents.push_back(agent);
}

void Apsis::World::Actor::attachImpeder(Apsis::Agent::Impeder& agent) {
  _impederAgents.push_back(agent);
}

void Apsis::World::Actor::update(double elapsed) {
  _currentTime += elapsed;
  if (_currentTime > 0.08) {
    nextFrame();
  }

  Apsis::Geometry::Point to;
  to.x = _position.x;
  to.y = _position.y;

  // update actor information based on the state
  if (strcmp(_state, "walk_up") == 0) {
    to.y += _moveRate * elapsed;
  }
  else if (strcmp(_state, "walk_down") == 0) {
    to.y -= _moveRate * elapsed;
  }
  else if (strcmp(_state, "walk_left") == 0) {
    to.x -= _moveRate * elapsed;
  }
  else if (strcmp(_state, "walk_right") == 0) {
    to.x += _moveRate * elapsed;
  }
  else {
    return;
  }
  
  for (unsigned int i = 0; i < _moverAgents.size(); i++) {
    if (_moverAgents[i].update(_position, to)) {
      this->move(to);
    }
  }
}

void Apsis::World::Actor::move(Apsis::Geometry::Point& to) {
  for (unsigned int i = 0; i < _impederAgents.size(); i++) {
    _impederAgents[i].update(_position, to);
  }

  _position.x = to.x;
  _position.y = to.y;
}

void Apsis::World::Actor::draw(glm::mat4& projection,
                               Primitives::Camera& camera,
                               glm::mat4& model) {
}