#include <apsis/world/object.h>

const Apsis::World::Value& Apsis::World::Object::get(const char* key) const {
  return this->get(Apsis::Registry::Property::id(key));
}

const Apsis::World::Value& Apsis::World::Object::get(unsigned int key) const {
  if (this->_properties.count(key) == 0) {
    throw "Key not found";
  }

  const Apsis::World::Value& value = this->_properties.at(key);
  return value;
}

void Apsis::World::Object::set(const char* key, double value) {
  this->set(Apsis::Registry::Property::id(key), value);
}

void Apsis::World::Object::set(unsigned int key, double value) {
  this->_properties[key] = World::Value(value);
}

void Apsis::World::Object::set(const char* key, long value) {
  this->set(Apsis::Registry::Property::id(key), value);
}

void Apsis::World::Object::set(unsigned int key, long value) {
  this->_properties[key] = World::Value(value);
}

bool Apsis::World::Object::isEnabled(unsigned int state) const {
  return this->_states.count(state) > 0;
}

void Apsis::World::Object::enableState(unsigned int state) {
  this->_states.insert(state);
}

void Apsis::World::Object::disableState(unsigned int state) {
  this->_states.erase(state);
}

bool Apsis::World::Object::toggleState(unsigned int state) {
  if (this->isEnabled(state)) {
    this->disableState(state);
    return false;
  }
  else {
    this->enableState(state);
    return true;
  }
}