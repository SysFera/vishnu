#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "constants.hpp"
#include "CommonParser.hpp"
#include "FileParser.hpp"

extern ConfigMap *configPtr;
// simplify code using global configuration map
#define CONFIGMAP (*configPtr)

/**
 * @class simple_cast_trait
 * @brief traits class used by simple cast that sets zero_value
 * by default to 0, specialize for each type that requires.
 */
template <typename T>
class simple_cast_traits {
public:
  static const T zero_value = 0;
};

template <>
class simple_cast_traits<std::string> {
public:
  static const std::string zero_value ;
};

/**
 * @brief poor's man lexical_cast
 * empty strings are handled by using a traits class
 * @param arg argument
 * @return properly casted argument
 */
template <typename T, typename S>
T simple_cast(const S& arg) {
  T output = simple_cast_traits<T>::zero_value;
  std::stringstream buffer;
  buffer << arg;
  buffer >> output;

  return output;
}

/**
 * @param[in]  param
 * @param[out] value result
 * @return param has been set or not
 */
template<typename T>
bool
getConfigValue(vishnu::param_type_t param, T& value)
{
  const std::string& key = (vishnu::params)[param].value;
  ConfigMap::iterator it = configPtr->find(key);
  if (configPtr->end() == it) {
    return false;
  } else {
    value = simple_cast<T>(it->second);
    return true;
  }
}

#define CONFIG_BOOL(x, y) getConfigValue<bool>((x), (y))
#define CONFIG_INT(x, y) getConfigValue<int>((x), (y))
#define CONFIG_ULONG(x, y) getConfigValue<unsigned long>((x), (y))
#define CONFIG_STRING(x, y) getConfigValue<std::string>((x), (y))
#define CONFIG_ADDRESS(x, y) getAddressConfigValue((x), (y))
#define CONFIG_AGENT(x, y) getAgentConfigValue((x), (y))

#endif /* _CONFIGURATION_HPP_ */
