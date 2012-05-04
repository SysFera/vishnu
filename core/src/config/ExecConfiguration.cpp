/**
 * \file ExecConfiguration.cpp
 */

#include <regex.h>
#include <stdexcept>
#include <unistd.h>

#include "ExecConfiguration.hpp"
#include "constants.hpp"

#define MAXPATHLEN 200   // make this larger if you need to.

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

string
ExecConfiguration::scriptToString(){
  string res;
  for (map<string, string>::iterator it = mconfig.begin();
       it != mconfig.end();
       ++it){
    res += it->first;
    res += "=";
    res += it->second;
    res += "\n";
  }
  return res;
}

string
ExecConfiguration::getCurrentBinaryDir()
{
  int length;
  char fullpath[MAXPATHLEN];

  /* /proc/self is a symbolic link to the process-ID subdir
  * of /proc, e.g. /proc/4323 when the pid of the process
  * of this program is 4323.
  *
  * Inside /proc/<pid> there is a symbolic link to the
  * executable that is running as this <pid>.  This symbolic
  * link is called "exe".
  *
  * So if we read the path where the symlink /proc/self/exe
  * points to we have the full path of the executable.
  */

  length = readlink("/proc/self/exe", fullpath, sizeof(fullpath));

  // Catch some errors
  if (length < 0) {
      cerr << "Error resolving symlink /proc/self/exe." << endl;
      exit(1);
  }
  if (length >= MAXPATHLEN) {
      cerr << "Path too long. Truncated." << endl;
      exit(1);
  }

  /* I don't know why, but the string this readlink() function
  * returns is appended with a '@'.
  */
  fullpath[length] = '\0';       // Strip '@' off the end.

  // remove the executable name to get only the directory
  string execFullPath = fullpath;
  string execDir = execFullPath.substr(0,execFullPath.find_last_of("/"));

  return execDir;
}

