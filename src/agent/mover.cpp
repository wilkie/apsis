#include "apsis/agent/mover.h"

#include "apsis/geometry/point.h"

#include <algorithm>

Apsis::Agent::Mover::Mover(char* rule) :
  _rule(rule) {

  // Register this agent by its rule
  _id = registerRule(rule);
}

bool Apsis::Agent::Mover::update(float elapsed,
                                 std::set<unsigned int>& states,
                                 const Apsis::Geometry::Rectangle& original,
                                 Apsis::Geometry::Point& updated) {
  return false;
}

bool Apsis::Agent::Mover::supercedes(const char* rule) {
  if (_supercedes.size() > 0) {
    return _supercedes.count(registerRule(rule)) > 0;
  }

  return _supercedes.count(registerRule(rule)) > 0;
}

char* Apsis::Agent::Mover::rule() {
  return _rule;
}

void Apsis::Agent::Mover::supercede(const char* rule) {
  _supercedes.insert(registerRule(rule));
}

std::vector<std::string> Apsis::Agent::Mover::_ids;

unsigned int Apsis::Agent::Mover::registerRule(const char* rule) {
  std::string str = std::string(rule);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return std::distance(_ids.begin(), it);
  }

  _ids.push_back(str);
  return _ids.size() - 1;
}