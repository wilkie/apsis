#include "apsis/registry/widget.h"

#include "apsis/engine/object.h"

#include "apsis/interface/label.h"
#include "apsis/interface/button.h"
#include "apsis/interface/check_box.h"

#include <algorithm>
#include <fstream>

using namespace Apsis;

std::vector<std::string> Registry::Widget::_ids;
std::vector<Registry::Widget*> Registry::Widget::_interfaces;

// Establish all of the known internal widgets

typedef std::map<std::string, Interface::Event::Init*>::value_type InitPair;

static InitPair pairs_inits[] = {
  InitPair("label", &Interface::Label::init),
  InitPair("button", &Interface::Button::init),
  InitPair("check_box", &Interface::CheckBox::init),
};

static const unsigned int inits_count = sizeof(pairs_inits) / sizeof(InitPair);

std::map<std::string, Interface::Event::Init*> Registry::Widget::_internal_inits(pairs_inits, pairs_inits + inits_count);

typedef std::map<std::string, Interface::Event::Draw*>::value_type DrawPair;

static DrawPair pairs_draws[] = {
  DrawPair("label", &Interface::Label::draw),
  DrawPair("button", &Interface::Button::draw),
  DrawPair("check_box", &Interface::CheckBox::draw),
};

static const unsigned int draws_count = sizeof(pairs_draws) / sizeof(DrawPair);

std::map<std::string, Interface::Event::Draw*> Registry::Widget::_internal_draws(pairs_draws, pairs_draws + draws_count);

typedef std::map<std::string, Interface::Event::Update*>::value_type UpdatePair;

static UpdatePair pairs_updates[] = {
  UpdatePair("label", &Interface::Label::update),
  UpdatePair("button", &Interface::Button::update),
  UpdatePair("check_box", &Interface::CheckBox::update),
};

static const unsigned int updates_count = sizeof(pairs_updates) / sizeof(UpdatePair);

std::map<std::string, Interface::Event::Update*> Registry::Widget::_internal_updates(pairs_updates, pairs_updates + updates_count);

typedef std::map<std::string, Interface::Event::Input*>::value_type InputPair;

static InputPair pairs_inputs[] = {
  InputPair("label", &Interface::Label::input),
  InputPair("button", &Interface::Button::input),
  InputPair("check_box", &Interface::CheckBox::input),
};

static const unsigned int inputs_count = sizeof(pairs_inputs) / sizeof(InputPair);

std::map<std::string, Interface::Event::Input*> Registry::Widget::_internal_inputs(pairs_inputs, pairs_inputs + inputs_count);

typedef std::map<std::string, Interface::Event::Enter*>::value_type EnterPair;

static EnterPair pairs_enters[] = {
  EnterPair("label", &Interface::Label::enter),
  EnterPair("button", &Interface::Button::enter),
  EnterPair("check_box", &Interface::CheckBox::enter),
};

static const unsigned int enters_count = sizeof(pairs_enters) / sizeof(EnterPair);

std::map<std::string, Interface::Event::Enter*> Registry::Widget::_internal_enters(pairs_enters, pairs_enters + enters_count);

typedef std::map<std::string, Interface::Event::Leave*>::value_type LeavePair;

static LeavePair pairs_leaves[] = {
  LeavePair("label", &Interface::Label::leave),
  LeavePair("button", &Interface::Button::leave),
  LeavePair("check_box", &Interface::CheckBox::leave),
};

static const unsigned int leaves_count = sizeof(pairs_leaves) / sizeof(LeavePair);

std::map<std::string, Interface::Event::Leave*> Registry::Widget::_internal_leaves(pairs_leaves, pairs_leaves + leaves_count);

typedef std::map<std::string, Interface::Event::Motion*>::value_type MotionPair;

static MotionPair pairs_motions[] = {
  MotionPair("label", &Interface::Label::motion),
  MotionPair("button", &Interface::Button::motion),
  MotionPair("check_box", &Interface::CheckBox::motion),
};

static const unsigned int motions_count = sizeof(pairs_motions) / sizeof(MotionPair);

std::map<std::string, Interface::Event::Motion*> Registry::Widget::_internal_motions(pairs_motions, pairs_motions + motions_count);

const Registry::Widget&
Registry::Widget::load(const char* path,
                       const Engine::Object& loader) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_interfaces[std::distance(_ids.begin(), it)];
  }

  _interfaces.push_back(new Registry::Widget(path, loader));
  _ids.push_back(str);
  return *_interfaces[_ids.size() - 1];
}

Registry::Widget::Widget(const char* path,
                         const Engine::Object& loader)
  : _jsonLoaded(false),
    _path(path),
    _init(_getInitFunction()),
    _draw(_getDrawFunction()),
    _update(_getUpdateFunction()),
    _motion(_getMotionFunction()),
    _leave_(_getLeaveFunction()),
    _enter(_getEnterFunction()),
    _input(_getInputFunction()) {

  _parseJSONFile();
}

void Registry::Widget::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;

  if (_value.isMember("inherit")) {
    // TODO: inherit widgets
  }

  if (_value.isMember("name")) {
    _name = _value["name"].asCString();
  }
}

void Registry::Widget::_parseJSONFile() {
  _openJSONFile();

  if (_value.isMember("type") &&
      _value["type"].compare(Json::Value("widget")) == 0) {
  }
  else {
    throw "Widget file given is not of type 'widget'.";
  }

  if (_value.isMember("properties")) {
    // Load properties
    for (Json::Value::iterator it = _value["properties"].begin();
         it != _value["properties"].end();
         ++it) {
      if ((*it).isObject()) {
        if ((*it).isMember("name") &&
            (*it).isMember("type") &&
            (*it).isMember("default")) {
          const char* name = (*it)["name"].asCString();
          const char* type = (*it)["type"].asCString();
          const char* def  = (*it)["default"].asCString();

          // Add this to the collection of properties
          _properties.push_back(name);
          _types.push_back(type);
          _values.push_back(def);
        }
      }
      else {
        throw "Widget file's 'properties' section is malformed.";
      }
    }
  }
}

Interface::Event::Init& Registry::Widget::_getInitFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_inits.count(_name) > 0) {
      return *_internal_inits.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultInit;
}

Interface::Event::Draw& Registry::Widget::_getDrawFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_draws.count(_name) > 0) {
      return *_internal_draws.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultDraw;
}

Interface::Event::Motion& Registry::Widget::_getMotionFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_motions.count(_name) > 0) {
      return *_internal_motions.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultMotion;
}

Interface::Event::Enter& Registry::Widget::_getEnterFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_enters.count(_name) > 0) {
      return *_internal_enters.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultEnter;
}

Interface::Event::Leave& Registry::Widget::_getLeaveFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_leaves.count(_name) > 0) {
      return *_internal_leaves.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultLeave;
}

Interface::Event::Input& Registry::Widget::_getInputFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_inputs.count(_name) > 0) {
      return *_internal_inputs.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultInput;
}

Interface::Event::Update& Registry::Widget::_getUpdateFunction() {
  _openJSONFile();
  if (_value.isMember("internal")) {
    // Get the internal function
    if (_internal_updates.count(_name) > 0) {
      return *_internal_updates.at(_name);
    }
  }
  return Apsis::Interface::Event::defaultUpdate;
}

Interface::Event::Init& Registry::Widget::initEvent() const {
  return _init;
}

Interface::Event::Draw& Registry::Widget::drawEvent() const {
  return _draw;
}

Interface::Event::Update& Registry::Widget::updateEvent() const {
  return _update;
}

Interface::Event::Motion& Registry::Widget::motionEvent() const {
  return _motion;
}

Interface::Event::Leave& Registry::Widget::leaveEvent() const {
  return _leave_;
}

Interface::Event::Enter& Registry::Widget::enterEvent() const {
  return _enter;
}

Interface::Event::Input& Registry::Widget::inputEvent() const {
  return _input;
}

const char* Registry::Widget::name() const {
  return _name.c_str();
}

unsigned int Registry::Widget::propertyCount() const {
  return _properties.size();
}

const char* Registry::Widget::propertyName(unsigned int index) const {
  return _properties[index].c_str();
}

const char* Registry::Widget::propertyDefault(unsigned int index) const {
  return _values[index].c_str();
}