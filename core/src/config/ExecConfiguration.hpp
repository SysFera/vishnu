/**
 * \file ExecConfiguration.hpp
 */

#ifndef _EXECCONFIGURATION_HPP_
#define _EXECCONFIGURATION_HPP_

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "constants.hpp"
#include "CommonParser.hpp"
#include "FileParser.hpp"
#include "UserException.hpp"

/**
 * \class simple_cast_traits
 * \brief traits class used by simple cast that sets zero_value
 * by default to 0, specialize for each type that requires.
 */
template <typename T>
class simple_cast_traits {
public:
  /**
   * \brief To store the zero value
   */
  static const T zero_value = 0;
};

/**
 * \brief A specialization of the simple_cast_traits class for string type
 */
template <>
class simple_cast_traits<std::string> {
public:
  /**
   * \brief To store the empty string
   */
  static const std::string zero_value ;
};

/**
 * \brief lexical_cast
 * empty strings are handled by using a traits class
 * \param arg argument
 * \return properly casted argument
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
 * \class ExecConfiguration
 * \brief Program configuration based on key/value pairs stored in a file
 */
class ExecConfiguration {

public:
  ExecConfiguration();

  /**
   * \brief Initialize from a file
   * \param filePath  full path of the configuration filePath
   */
  void initFromFile(std::string filePath) throw (UserException,std::exception);

  /**
   * \brief Get the value of a configuration parameter
   * \param[in]  param
   * \param[out] value result
   * \return param has been set or not
   */
  template<typename T>
  bool
  getConfigValue(vishnu::param_type_t param, T& value) const {
    const std::string& key = (vishnu::params)[param].value;
    ConfigMap::const_iterator it = mconfig.find(key);
    if (mconfig.end() == it) {
      return false;
    } else {
      value = simple_cast<T>(it->second);
      return true;
    }
  }

  /**
   * \brief Check and get the value of a configuration parameter
   * \param[in]  param
   * \param[out] value result
   */
  template<typename T>
  void
  getRequiredConfigValue(vishnu::param_type_t param, T& value) const throw (UserException)
  {
    if (!getConfigValue<T>(param, value)) {
      throw UserException(ERRCODE_CONFIGNOTFOUND, vishnu::params[param].value);
    }
  }

  /**
   * \brief To convert the script in a string
   */
  std::string
  scriptToString();

  /**
   * \brief To get the full path to the current executable
   */
  static std::string
  getCurrentBinaryDir();

protected:

  /**
   * \brief To store the configuration map
   */

  ConfigMap mconfig;
};

#endif /* _EXECCONFIGURATION_HPP_ */
