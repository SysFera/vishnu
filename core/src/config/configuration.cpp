#include <regex.h>
#include <stdexcept>

#include "configuration.hpp"
#include "constants.hpp"

const std::string simple_cast_traits<std::string>::zero_value = "";

/**
 * @param[in]  param
 * @param[out] value result
 * @return param has been set or not
 */
template<>
bool
getConfigValue(vishnu::param_type_t param, std::string& value)
{
  const std::string& key = (vishnu::params)[param].value;
  ConfigMap::iterator it = configPtr->find(key);
  if (configPtr->end() == it) {
    return false;
  } else {
    value = it->second;
    return true;
  }
}
