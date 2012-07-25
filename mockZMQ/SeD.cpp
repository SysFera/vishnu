#include "SeD.hpp"

int
SeD::call(diet_profile_t* profile) {
  std::map<std::string, functionPtr_t>::iterator it;
  it = mcb.find(profile->name);
  if (it == mcb.end()) {
    std::cerr << "not found service : " << profile->name << "\n";
    return UNKNOWN_SERVICE;
  }
  int (*functionPtr)(diet_profile_t*);
  functionPtr = it->second;
  return (*functionPtr)(profile);
}
