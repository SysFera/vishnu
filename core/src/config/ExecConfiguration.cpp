/**
 * \file ExecConfiguration.cpp
 */

#include <regex.h>
#include <stdexcept>

#include "ExecConfiguration.hpp"
#include "constants.hpp"

const std::string simple_cast_traits<std::string>::zero_value = "";

/**
 * \brief Constructor
 */
ExecConfiguration::ExecConfiguration()
{
}


/**
 * \brief Initialize from a file
 * \param filePath  full path of the configuration filePath
 */
void ExecConfiguration::initFromFile(std::string filePath) throw (UserException)
{
  FileParser fileParser;
  fileParser.parseFile(filePath);
  mconfig = fileParser.getConfiguration();
}


/**
 * \brief Get the value of a configuration parameter
 * \param[in]  param
 * \param[out] value result
 * \return param has been set or not
 */
template<>
bool
ExecConfiguration::getConfigValue(vishnu::param_type_t param, std::string& value) const
{
  const std::string& key = (vishnu::params)[param].value;
  ConfigMap::const_iterator it = mconfig.find(key);
  if (mconfig.end() == it) {
    return false;
  } else {
    value = it->second;
    return true;
  }
}
