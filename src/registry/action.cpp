#include <apsis/registry/action.h>

#include <algorithm>
#include <fstream>
#include <json/json.h>

std::vector<std::string> Apsis::Registry::Action::_ids;
std::vector<Apsis::Registry::Action*> Apsis::Registry::Action::_actions;

std::vector<std::string> Apsis::Registry::Action::_paths;

Apsis::Registry::Action& Apsis::Registry::Action::id(const char* name) {
  std::string str = std::string(name);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_actions[std::distance(_ids.begin(), it)];
  }
  
  _actions.push_back(new Apsis::Registry::Action());
  _ids.push_back(str);
  _actions[_ids.size() - 1]->_id = _ids.size()-1;
  return *_actions[_ids.size() - 1];
}

unsigned int Apsis::Registry::Action::id() const {
  return _id;
}

Apsis::Registry::Action& Apsis::Registry::Action::id(unsigned int id) {
  return *_actions[id];
}

unsigned int Apsis::Registry::Action::count() {
  return _actions.size();
}

const Apsis::Input::Binding& Apsis::Registry::Action::primary() const {
  if (_bindings.size() > 0) {
    return _bindings[0];
  }
  else {
    // TODO: i18n
    throw "No primary binding.";
  }
}

const Apsis::Input::Binding& Apsis::Registry::Action::secondary() const {
  if (_bindings.size() > 1) {
    return _bindings[1];
  }
  else {
    // TODO: i18n
    throw "No secondary binding.";
  }
}

void Apsis::Registry::Action::bindPrimary(Apsis::Input::Binding& binding) {
  if (_bindings.size() > 0) {
    _bindings[0] = binding;
  }
  else {
    _bindings.push_back(binding);
  }
}

void Apsis::Registry::Action::bindSecondary(Apsis::Input::Binding& binding) {
  if (_bindings.size() > 1) {
    _bindings[1] = binding;
  }
  else {
    _bindings.push_back(binding);
  }
}

unsigned int Apsis::Registry::Action::bindingCount() const {
  return _bindings.size();
}

void Apsis::Registry::Action::clearPrimary() {
  if (_bindings.size() > 0) {
    _bindings.erase(_bindings.begin());
  }
}

void Apsis::Registry::Action::clearSecondary() {
  if (_bindings.size() > 1) {
    _bindings.erase(_bindings.begin() + 1);
  }
}

void Apsis::Registry::Action::clear() {
  _bindings.clear();
}

bool Apsis::Registry::Action::yield(const Apsis::Input::Binding& binding,
                                    unsigned int& action_id) {
  for (unsigned int i = 0; i < _actions.size(); i++) {
    const Apsis::Registry::Action& action = *_actions[i];

    for (unsigned int k = 0; k < action._bindings.size(); k++) {
      const Apsis::Input::Binding& sub_binding = action._bindings[k];

      if (sub_binding.key()     == binding.key()     &&
          sub_binding.shift()   == binding.shift()   &&
          sub_binding.control() == binding.control() &&
          sub_binding.alt()     == binding.alt()) {
        action_id = i;
        return true;
      }
    }
  }

  return false;
}

void Apsis::Registry::Action::load(const char* path) {
  Json::Reader reader;
  Json::Value  value;

  _paths.push_back(path);

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  if (value.isMember("type") && (value["type"].compare(Json::Value("binding")) == 0)) {
    for (Json::Value::iterator it = value["bindings"].begin();
         it != value["bindings"].end();
         ++it) {
      std::string action_name = (*it)["action"].asCString();

      Apsis::Registry::Action& action = Apsis::Registry::Action::id(action_name.c_str());

      for (Json::Value::iterator it_key = (*it)["keys"].begin();
           it_key != (*it)["keys"].end();
           ++it_key) {
        if ((*it_key).isObject()) {
          const char* code;
          if ((*it_key).isMember("code")) {
            code    = (*it_key)["code"].asCString();
          }
          else {
            throw "Binding description must have a code field for every item.";
          }

          bool shift   = false;
          bool alt     = false;
          bool control = false;

          if ((*it_key).isMember("shift")) {
            shift   = (*it_key)["shift"].asBool();
          }
          if ((*it_key).isMember("alt")) {
            alt     = (*it_key)["alt"].asBool();
          }
          if ((*it_key).isMember("control")) {
            control = (*it_key)["control"].asBool();
          }

          if (action.bindingCount() < 2) {
            action.bindSecondary(
              Apsis::Input::Binding(code, shift, control, alt));
          }
        }
        else {
          if (action.bindingCount() < 2) {
            action.bindSecondary(
              Apsis::Input::Binding((*it_key).asCString(), false, false, false));
          }
        }
      }
    }
  }
  else {
    throw "Loaded JSON that does not describe bindings.";
  }
}