#include "apsis/sprite/thing.h"

#include "apsis/registry/event.h"

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
    if (inherited._animations.size() > 0) {
      _animations = inherited._animations;
    }
    if (inherited._collideFunctions.size() > 0) {
      _collideFunctions = inherited._collideFunctions;
    }

    const Apsis::World::RuleSet& rules = inherited.rules();
    for (unsigned int i = 0; i < rules.count(); i++) {
      _rules.addRule(rules.rule(i));
    }

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

  if (_value.isMember("name")) {
    _name = _value["name"].asCString();
  }
  else {
    // Get name from path
    size_t start = _path.find_last_of('/');
    if (start == std::string::npos) {
      start = 0;
    }
    else {
      start ++;
    }
    size_t end   = _path.find_last_of('.');
    if (end == std::string::npos) {
      _name = _path.substr(start);
    }
    else {
      _name = _path.substr(start, end-start);
    }
  }

  if (_value.isMember("width")) {
    _object.set("width",  _value["width"].asDouble());
  }

  if (_value.isMember("height")) {
    _object.set("height", _value["height"].asDouble());
  }

  // Animation
  if (_value.isMember("animations")) {
    // TODO: better handling of invalid values
    for (Json::Value::iterator it = _value["animations"].begin();
         it != _value["animations"].end();
         ++it) {
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

  // Events (collision)
  if (_value.isMember("on_collides")) {
    // TODO: better handling of invalid values
    for (Json::Value::iterator it = _value["on_collides"].begin();
         it != _value["on_collides"].end();
         ++it) {
      std::string event_name = "collide_with_";
      std::string rule_name  = "";

      unsigned int event_id = 0;
      // get collision event name
      if (!(*it).isObject()) {
        throw "Thing description's 'on_collides' array does not contain objects.";
      }

      if ((*it).isMember("with")) {
        if (!(*it)["with"].isObject()) {
          throw "Thing description's 'on_collide' has a member where 'with' is not an object.";
        }

        if ((*it)["with"].isMember("type") &&
            (*it)["with"].isMember("name")) {
          event_name = event_name.append((*it)["with"]["type"].asCString());
          event_name = event_name.append("_");
          event_name = event_name.append((*it)["with"]["name"].asCString());

          event_id = Apsis::Registry::Event::id(event_name.c_str());

          // Tell the object we respond to this event
          _object.respondTo(event_id);
        }
        else {
          throw "Thing description's 'on_collide' members should have a 'with' object with 'type' and 'name fields.";
        }
      }
      else {
        throw "Thing description's 'on_collide' has a member without a 'with' object.";
      }

      if ((*it).isMember("do")) {
        if (!(*it)["do"].isString()) {
          throw "Thing description's 'on_collide' has a member where the 'do' field is not a string.";
        }

        rule_name = (*it)["do"].asCString();

        std::string rule_path = "assets/rules/" + std::string(rule_name) + ".json";

        _rules.addRule(Registry::Rule::load(rule_path.c_str()));
      }
      else {
        throw "Thing description's 'on_collide' has a member without a 'do' object.";
      }

      if ((*it).isMember("properties")) {
        if (!(*it)["properties"].isObject()) {
          throw "Thing description's 'on_collide' has a member where 'properties' is not an object.";
        }

        Json::Value::Members members = (*it)["properties"].getMemberNames();
        for (Json::Value::Members::iterator property_it = members.begin();
             property_it != members.end();
             ++property_it) {
          const char* name = (*property_it).c_str();
          unsigned int property_id = Apsis::Registry::Property::id(name);

          if ((*it)["properties"][name].isDouble()) {
            _object.setForEvent(event_id, property_id, (*it)["properties"][name].asDouble());
          }
          else if ((*it)["properties"][name].isString()) {
            _object.setForEvent(event_id, property_id, (*it)["properties"][name].asCString());
          }
          else if ((*it)["properties"][name].isIntegral()) {
            _object.setForEvent(event_id, property_id, (long)(*it)["properties"][name].asInt());
          }
        }
      }
    }
  }

  // Rules
  if (_value.isMember("rules")) {
    // TODO: better handling of invalid values
    for (Json::Value::iterator it = _value["rules"].begin();
         it != _value["rules"].end();
         ++it) {
      std::string rule_path;
      if ((*it).isObject()) {
        rule_path = (*it)["name"].asCString();
      }
      else if ((*it).isString()) {
        rule_path = (*it).asCString();
      }

      rule_path = "assets/rules/" + rule_path + ".json";

      _rules.addRule(Registry::Rule::load(rule_path.c_str()));
    }
  }

  // Properties
  if (_value.isMember("properties") && _value["properties"].isObject()) {
    // TODO: better handling of invalid values
    Json::Value::Members members = _value["properties"].getMemberNames();
    for (Json::Value::Members::iterator it = members.begin();
         it != members.end();
         ++it) {
      const char* name = (*it).c_str();
      unsigned int property_id = Apsis::Registry::Property::id(name);

      if (_value["properties"][name].isDouble()) {
        _object.set(property_id, _value["properties"][name].asDouble());
      }
      else if (_value["properties"][name].isString()) {
        _object.set(property_id, _value["properties"][name].asCString());
      }
      else if (_value["properties"][name].isIntegral()) {
        _object.set(property_id, (long)_value["properties"][name].asInt());
      }
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

const Apsis::World::RuleSet& Apsis::Sprite::Thing::rules() const {
  return _rules;
}

const char* Apsis::Sprite::Thing::name() const {
  return _name.c_str();
}