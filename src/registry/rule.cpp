#include "apsis/registry/rule.h"

#include <fstream>
#include <algorithm>

#include "apsis/rules/act_function.h"
#include "apsis/rules/update_function.h"
#include "apsis/rules/collide_function.h"

#include "apsis/rules/slide_map_collider.h"
#include "apsis/rules/map_collider.h"
#include "apsis/rules/actor_collider.h"

#include "apsis/rules/wiggler.h"
#include "apsis/rules/right.h"
#include "apsis/rules/left.h"
#include "apsis/rules/up.h"
#include "apsis/rules/down.h"
#include "apsis/rules/fall.h"

std::vector<std::string> Apsis::Registry::Rule::_ids;
std::vector<Apsis::Registry::Rule*> Apsis::Registry::Rule::_all_rules;

// Establish all of the known rules

typedef std::map<std::string, Apsis::Rules::UpdateFunction>::value_type UpdaterPair;
typedef std::map<std::string, Apsis::Rules::CollideFunction>::value_type ColliderPair;
typedef std::map<std::string, Apsis::Rules::ActFunction>::value_type ActFunctionPair;
typedef std::map<std::string, std::string>::value_type ActStringPair;

static UpdaterPair pairs_updaters[] = {
  UpdaterPair("wiggler",   &Apsis::Rules::Wiggler::update),
  UpdaterPair("right",     &Apsis::Rules::Right::update),
  UpdaterPair("left",      &Apsis::Rules::Left::update),
  UpdaterPair("up",        &Apsis::Rules::Up::update),
  UpdaterPair("down",      &Apsis::Rules::Down::update),
  UpdaterPair("fall",      &Apsis::Rules::Fall::update),
};

static const unsigned int updaters_count = sizeof(pairs_updaters) / sizeof(UpdaterPair);

std::map<std::string, Apsis::Rules::UpdateFunction> Apsis::Registry::Rule::_internal_updates(pairs_updaters, pairs_updaters + updaters_count);

static ColliderPair pairs_colliders[] = {
  ColliderPair("slide_map_collider",   &Apsis::Rules::SlideMapCollider::collide),
  ColliderPair("map_collider",   &Apsis::Rules::MapCollider::collide),
  ColliderPair("actor_collider", &Apsis::Rules::ActorCollider::collide),
};

static const unsigned int colliders_count = sizeof(pairs_colliders) / sizeof(ColliderPair);

std::map<std::string, Apsis::Rules::CollideFunction> Apsis::Registry::Rule::_internal_collides(pairs_colliders, pairs_colliders + colliders_count);

static ActFunctionPair pairs_acts[] = {
  ActFunctionPair("right", &Apsis::Rules::Right::act),
  ActFunctionPair("left",  &Apsis::Rules::Left::act),
  ActFunctionPair("up",  &Apsis::Rules::Up::act),
  ActFunctionPair("down",  &Apsis::Rules::Down::act),
};

static ActStringPair pairs_act_actions[] = {
  ActStringPair("right", Apsis::Rules::Right::action),
  ActStringPair("left",  Apsis::Rules::Left::action),
  ActStringPair("up",  Apsis::Rules::Up::action),
  ActStringPair("down",  Apsis::Rules::Down::action),
};

static const unsigned int acts_count = sizeof(pairs_acts) / sizeof(ActFunctionPair);

std::map<std::string, Apsis::Rules::ActFunction> Apsis::Registry::Rule::_internal_acts(pairs_acts, pairs_acts + acts_count);

std::map<std::string, std::string> Apsis::Registry::Rule::_internal_act_actions(pairs_act_actions, pairs_act_actions + acts_count);

const Apsis::Registry::Rule& Apsis::Registry::Rule::load(const char* path) {
  // TODO: Only use names with relative paths. Use a class to handle pathing.

  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_all_rules[std::distance(_ids.begin(), it)];
  }

  _all_rules.push_back(new Apsis::Registry::Rule(path));
  _ids.push_back(str);
  return *_all_rules[_ids.size() - 1];
}

Apsis::Registry::Rule::Rule(const char* path) 
  : _path(path),
    _jsonLoaded(false) {
  _parseJSONFile();
}

const char* Apsis::Registry::Rule::name() const {
  return _name.c_str();
}

const char* Apsis::Registry::Rule::path() const {
  return _path.c_str();
}

unsigned int Apsis::Registry::Rule::ruleCount() const {
  return _rules.size();
}

const Apsis::Registry::Rule& Apsis::Registry::Rule::rule(unsigned int id) const {
  return *_rules[id];
}

unsigned int Apsis::Registry::Rule::collideFunctionCount() const {
  return _collides.size();
}

Apsis::Rules::CollideFunction Apsis::Registry::Rule::collideFunction(unsigned int id) const {
  return _collides[id];
}

unsigned int Apsis::Registry::Rule::updateFunctionCount() const {
  return _updates.size();
}

Apsis::Rules::UpdateFunction Apsis::Registry::Rule::updateFunction(unsigned int id) const {
  return _updates[id];
}

unsigned int Apsis::Registry::Rule::actFunctionCount() const {
  return _acts.size();
}

Apsis::Rules::ActFunction Apsis::Registry::Rule::actFunction(unsigned int id) const {
  return _acts[id];
}

unsigned int Apsis::Registry::Rule::actionId(unsigned int id) const {
  return _action_ids[id];
}

const char* Apsis::Registry::Rule::replacement(unsigned int id) const {
  return _supercedes[id].c_str();
}

unsigned int Apsis::Registry::Rule::replacementCount() const {
  return _supercedes.size();
}

bool Apsis::Registry::Rule::supercedes(const char* rule) const {
  if (_supercedes.size() == 0) {
    return false;
  }

  std::string str = rule;
  std::vector<std::string>::const_iterator it = std::find(_supercedes.begin(), _supercedes.end(), str);

  return (it != _ids.end());
}

void Apsis::Registry::Rule::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;

  if (_value.isMember("inherit")) {
    // Load inherited rule. This will base the given rule on an existing rule.
    const Apsis::Registry::Rule& inherited
      = Apsis::Registry::Rule::load(_value["inherit"].asCString());

    _inherited = &inherited;
  }
}

void Apsis::Registry::Rule::_parseJSONFile() {
  // Open JSON (if necessary)
  _openJSONFile();

  if (_value.isMember("name")) {
    // Retrieve name. In the system, the base name of the file is
    // going to be the default name.
    _name = _value["name"].asCString();
  }

  if (_value.isMember("bindings")) {
    // Load bindings, which indicate keyboard bindings that are used by this
    // rule and should be added to the bindings repository.
    for (Json::Value::iterator it = _value["bindings"].begin();
         it != _value["bindings"].end();
         ++it) {
    }
  }

  if (_value.isMember("supercedes")) {
    // Load supercedes, which indicate rules that, if used, should only
    // be triggered after checking if this rule should apply first.
    for (Json::Value::iterator it = _value["supercedes"].begin();
         it != _value["supercedes"].end();
         ++it) {
      _supercedes.push_back((*it).asCString());
    }
  }

  if (_value.isMember("internal")) {
    if (_value["internal"].asBool() == true) {
      // This rule is implemented internally
      if (_internal_collides.count(_name) > 0) {
        _collides.push_back(_internal_collides[_name]);
      }
      if (_internal_updates.count(_name) > 0) {
        _updates.push_back(_internal_updates[_name]);
      }
      if (_internal_acts.count(_name) > 0) {
        _acts.push_back(_internal_acts[_name]);
        _action_ids.push_back(Apsis::Registry::Action::id(_internal_act_actions[_name].c_str()).id());
      }
    }
  }
}