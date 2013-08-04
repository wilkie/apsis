#include <apsis/agent/responder.h>

#include <algorithm>

Apsis::Agent::Responder::Responder(const char* rule) :
  _rule(rule) {

  // Register this agent by its rule
  _id = registerRule(rule);
}


bool Apsis::Agent::Responder::uponEvent(std::set<unsigned int>& states,
                                        Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& intended) {
  return false;
}

bool Apsis::Agent::Responder::supercedes(const char* rule) {
  if (_supercedes.size() > 0) {
    return _supercedes.count(registerRule(rule)) > 0;
  }

  return _supercedes.count(registerRule(rule)) > 0;
}

const char* Apsis::Agent::Responder::rule() {
  return _rule;
}

void Apsis::Agent::Responder::supercede(const char* rule) {
  _supercedes.insert(registerRule(rule));
}

std::vector<std::string> Apsis::Agent::Responder::_ids;

unsigned int Apsis::Agent::Responder::registerRule(const char* rule) {
  std::string str = std::string(rule);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return std::distance(_ids.begin(), it);
  }

  _ids.push_back(str);
  return _ids.size() - 1;
}