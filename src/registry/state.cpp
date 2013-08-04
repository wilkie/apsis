#include <apsis/registry/state.h>

#include <algorithm>

std::vector<std::string> Apsis::Registry::State::_ids;

unsigned int Apsis::Registry::State::id(const char* name) {
  std::string str = std::string(name);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return std::distance(_ids.begin(), it);
  }

  _ids.push_back(str);
  return _ids.size() - 1;
}