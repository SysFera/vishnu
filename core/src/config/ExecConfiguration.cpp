/**
 * \file ExecConfiguration.cpp
 */

#include "ExecConfiguration.hpp"

#include <boost/algorithm/string.hpp>

#include "constants.hpp"
#include "FileParser.hpp"

using namespace std;

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
void ExecConfiguration::initFromFile(std::string filePath) throw (UserException, std::exception)
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

/**
 * \brief Get the values of a configuration parameter
 * \param[in]  param
 * \param[out] value the list of result
 * \return param has been set or not
 */
bool
ExecConfiguration::getConfigValues(vishnu::param_type_t param, std::vector<std::string>& values) const {
  const std::string& key = (vishnu::params)[param].value;
  ConfigMap::const_iterator it = mconfig.find(key);
  if (mconfig.end() == it) {
    return false;
  }
  boost::split(values, it->second, boost::is_any_of(";"));
  return true;
}

/**
 * \brief Get the values of a configuration parameter
 * \param[in]  key
 * \param[out] value the list of result
 * \return values has been set or not
 */

bool
ExecConfiguration::getConfigValues(std::string& key, std::vector<std::string>& values) const {
  ConfigMap::const_iterator it = mconfig.find(key);
  if (mconfig.end() == it) {
    return false;
  }
  boost::split(values, it->second, boost::is_any_of(";"));
  return true;
}


string
ExecConfiguration::scriptToString() const {
  string res;
  for (ConfigMap::const_iterator it = mconfig.begin();
       it != mconfig.end();
       ++it) {
    res += it->first;
    res += "=";
    res += it->second;
    res += "\n";
  }
  return res;
}
