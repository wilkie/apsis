#include <apsis/world/object.h>

Apsis::World::Value Apsis::World::Object::get(const char* key) {
  return this->get(Apsis::Registry::Property::id(key));
}

Apsis::World::Value Apsis::World::Object::get(unsigned int key) {
  if ((this->_properties.size()-1 > key) || 
      (this->_hasValue[key] == false)) {
    throw "Key not found";
  }

  return this->_properties[key];
}

void Apsis::World::Object::set(unsigned int key, Apsis::World::Value& value) {
  while (this->_hasValue.size()-1 > key) {
    this->_hasValue.push_back(false);
  }

  this->_hasValue[key] = true;
  this->_properties[key] = value;
}

bool Apsis::World::Object::isEnabled(unsigned int state) {
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