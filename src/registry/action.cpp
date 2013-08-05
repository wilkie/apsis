#include <apsis/registry/action.h>

#include <algorithm>

std::vector<std::string> Apsis::Registry::Action::_ids;

unsigned int Apsis::Registry::Action::id(const char* name) {
  std::string str = std::string(name);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return std::distance(_ids.begin(), it);
  }

  _ids.push_back(str);
  return _ids.size() - 1;
}