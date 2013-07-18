#include "apsis/world/actor.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

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
          frame->spriteIndex = spriteIndex;
          newAnimation->frames.push_back(frame);
        }
      } while(spriteIndex != -1);
    }
  }
  fclose(f);

  _currentAnimation = _animations[0];
  _frame = _currentAnimation->frames[0];

  unsigned int sprite_count = _spriteSheet->count();

  unsigned int vertices_size = 4 * sprite_count;
  unsigned int elements_size = 6 * sprite_count;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];
  
  unsigned int i = 0;
  unsigned int ei = 0;
  unsigned int ti = 0;

  for (unsigned int si = 0; si < sprite_count; si++) {
    float coords[4];
    _spriteSheet->textureCoordinates(si, coords);
    Apsis::Primitives::Sprite* sprite = _spriteSheet->sprite(si); 

    _vertices[i * 5 + 0] = -(float)sprite->center_x + 200.0f;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + 200.0f;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;

    _vertices[i * 5 + 0] = -(float)sprite->center_x + 200.0f + (float)sprite->width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + 200.0f;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;
      
    _vertices[i * 5 + 0] = -(float)sprite->center_x + 200.0f + (float)sprite->width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + 200.0f + (float)sprite->height;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;
      
    _vertices[i * 5 + 0] = -(float)sprite->center_x + 200.0f;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + 200.0f + (float)sprite->height;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;

    _elements[ei] = i-4; ei++;
    _elements[ei] = i-3; ei++;
    _elements[ei] = i-1; ei++;

    _elements[ei] = i-3; ei++;
    _elements[ei] = i-2; ei++;
    _elements[ei] = i-1; ei++;
  }

  _vbo.transfer(_vertices, 5 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/flat.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 5, 0);
  program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 5, 3);

  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);

  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("tex", 0);
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

void Apsis::World::Actor::update(float elapsed) {
  _currentTime += elapsed;
  if (_currentTime > 0.08f) {
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
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);
  
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("camera", camera.eye());
  _vao.drawRange(_frame->spriteIndex * 6, 6);
}