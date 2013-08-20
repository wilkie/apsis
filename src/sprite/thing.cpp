#include "apsis/sprite/thing.h"

#include <algorithm>
#include <fstream>

std::vector<std::string> Apsis::Sprite::Thing::_ids;
std::vector<Apsis::Sprite::Thing*> Apsis::Sprite::Thing::_things;

Apsis::Sprite::Thing::Thing(const char* path)
  : _jsonLoaded(false),
    _path(path),
    _sheet(Apsis::Sprite::Thing::_loadSpriteSheet()) {
  
  _id = _things.size();

  _parseJSONFile();
}

unsigned int Apsis::Sprite::Thing::id() const {
  return _id;
}

const Apsis::Sprite::Thing& Apsis::Sprite::Thing::get(unsigned int id) {
  return *_things[id];
}

const Apsis::Sprite::Thing& Apsis::Sprite::Thing::load(const char* path) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_things[std::distance(_ids.begin(), it)];
  }

  _things.push_back(new Apsis::Sprite::Thing(path));
  _ids.push_back(str);
  return *_things[_ids.size() - 1];
}

const Apsis::Sprite::Sheet& Apsis::Sprite::Thing::sheet() const {
  return _sheet;
}
  
const Apsis::World::Object& Apsis::Sprite::Thing::object() const {
  return _object;
}

void Apsis::Sprite::Thing::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;

  if (_value.isMember("inherit")) {
    const Apsis::Sprite::Thing& inherited = Apsis::Sprite::Thing::load(_value["inherit"].asCString());
    _object = inherited._object;
    _animations = inherited._animations;
    _moverAgents = inherited._moverAgents;
    _impederAgents = inherited._impederAgents;

    _inherited = &inherited;
  }
}

const Apsis::Sprite::Sheet& Apsis::Sprite::Thing::_loadSpriteSheet() {
  _openJSONFile();

  if (_value.isMember("inherit")) {
    return _inherited->sheet();
  }
  return Apsis::Sprite::Sheet::load(_value["sprites"].asCString());
}

void Apsis::Sprite::Thing::_parseJSONFile() {
  _openJSONFile();

  if (_value.isMember("width")) {
    _object.set("width",  _value["width"].asDouble());
  }

  if (_value.isMember("height")) {
    _object.set("height", _value["height"].asDouble());
  }

  // Animation
  if (_value.isMember("animations")) {
    // TODO: better handling of invalid values
    for (Json::Value::iterator it = _value["animations"].begin(); it != _value["animations"].end(); it++) {
      // Create an animation structure
      Sprite::Animation newAnimation((*it)["name"].asCString());

      // Store all of the sprites
      int spriteIndex = -1;
      do {
        spriteIndex = _sheet.enumerateSprites((*it)["sprites"].asCString(), spriteIndex+1);
        if (spriteIndex != -1) {
          newAnimation.addFrame(spriteIndex);
        }
      } while(spriteIndex != -1);

      _animations.push_back(newAnimation);
    }
  }

  // Rules
  if (_value.isMember("rules")) {
    // TODO: better handling of invalid values
    for (Json::Value::iterator it = _value["rules"].begin(); it != _value["rules"].end(); it++) {
    }
  }
}

const Apsis::Sprite::Animation& Apsis::Sprite::Thing::animation(const char* name) const {
  // Look-up the animation
  for (unsigned int i = 0; i < _animations.size(); i++) {
    if (_animations[i].name() == std::string(name)) {
      return _animations[i];
    }
  }

  return _animations[0];
}

unsigned int Apsis::Sprite::Thing::animationId(const char* name) const {
  // Look-up the animation
  for (unsigned int i = 0; i < _animations.size(); i++) {
    if (_animations[i].name() == std::string(name)) {
      return i;
    }
  }

  return 0;
}

const Apsis::Sprite::Animation& Apsis::Sprite::Thing::animationById(unsigned int id) const {
  return _animations.at(id);
}

unsigned int Apsis::Sprite::Thing::animationCount() const {
  return _animations.size();
}