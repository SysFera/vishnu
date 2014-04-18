/*
 * tmsUtils.cpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */

#include "tmsUtils.hpp"
#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "constants.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <pwd.h>
#include <fstream>
#include <algorithm>
#include <iterator>

static const unsigned int MAXPATHLEN = 255;   // make this larger if you need to.


/**
 * \brief Function to convert a string to a batch type
 * \param  batchName the name of the batch type
 * \return BatchType value of the corresponding string.
 */
BatchType
vishnu::convertToBatchType(const std::string& batchName) {

  std::map<std::string, int>::const_iterator batch = BATCH_NAME_TO_TYPE_MAP.find(batchName);
  if (batch != BATCH_NAME_TO_TYPE_MAP.end()) {
    return static_cast<BatchType>(batch->second);
  }
  return UNDEFINED;
}


/**
     * \brief Function to convert the batch type to string
     * \param BatchType the batch type to convert
     * \return the converted batch type
     */
std::string vishnu::convertBatchTypeToString(BatchType batchType) {

  std::map<std::string, int>::const_iterator batch = BATCH_NAME_TO_TYPE_MAP.begin();

  while (batch != BATCH_NAME_TO_TYPE_MAP.end() && batch->second != batchType) {
    ++batch;
  }
  if (batch != BATCH_NAME_TO_TYPE_MAP.end()) {
    return batch->first;
  }
  return "UNKNOWN_BATCH_TYPE";
}

/**
 * \brief  function to convert job status into string
 * \param state: The state of job
 * \return The converted state value
 */
std::string
vishnu::convertJobStateToString(int state) {

  std::map<int, std::string>::const_iterator it = JOB_STATE_TO_NAME_MAP.find(state);
  if (it != JOB_STATE_TO_NAME_MAP.end()) {
    return it->second;
  }
  return "INVALID_JOB_STATE";
}

/**
 * \brief Function a given walltime into string
 * in the form [days:]hours:minutes:seconds (each on two digits)
 * \param walltime The walltime to convert
 * \return the walltime converted to string
 */
std::string
vishnu::convertWallTimeToString(const long& walltime) {
  long totalTime = walltime;
  long j = totalTime/86400;
  totalTime = walltime-j*86400;
  long h =  totalTime/3600;
  totalTime =  totalTime-h*3600;
  long m =  totalTime/60;
  long s =  totalTime-m*60;

  std::ostringstream StrWallTime;
  if (j > 0) {
    if (j < 10) {
      StrWallTime << "0" << j << ":";
    } else {
      StrWallTime <<  j << ":";
    }
  }

  if (h < 10) {
    StrWallTime << "0" << h << ":";
  } else {
    StrWallTime <<  h << ":";
  }

  if (m < 10) {
    StrWallTime << "0" << m << ":";
  } else {
    StrWallTime << m << ":";
  }

  if(s < 10) {
    StrWallTime << "0" << s;
  } else {
    StrWallTime << s;
  }

  return StrWallTime.str();
}

/**
 * \brief Function a given walltime into seconds
 *
 * walltime can be of the following format:
 * - it can start and/or end by '"'
 * - each value is separated by ':'
 * - possible values represent:
 *   - days
 *   - hours
 *   - minutes
 *   - seconds
 * in the form [days:][hours:][minutes:]seconds
 *  (element between square brackets are optional)
 *
 * \param walltime The walltime to convert
 * \return the walltime converted to seconds
 */
long
vishnu::convertStringToWallTime(const std::string& walltime) {
  boost::regex reg("^\"?(\\d+)(?::(\\d+))?(?::(\\d+))?(?::(\\d+))?\"?$");
  boost::smatch groups;

  long time = 21600; // Fix default value to 6 hour

  if (walltime.empty()) {
    return time;  // Default value;
  }

  if (boost::regex_match(walltime, groups, reg)) {
    boost::smatch::const_iterator it;
    unsigned int start = 1;  // skip first value which is the full string

    it = groups.begin();
    ++it;  // skip first value which is the full string
    /* Count the number of not empty groups (-1, as the first element
     * in the group is the full string)
     */
    unsigned int nbFound = groups.size() - 1 - std::count(it, groups.end(), "");

    if (nbFound == 4) {  // days:hours:minutes:seconds
      time += 86400 * boost::lexical_cast<long>(groups[1]);
      start = 2;
    }

    for (unsigned int i = start; i <= nbFound; i++) {
      time += boost::lexical_cast<long>(
                std::pow(60.,
                         boost::lexical_cast<int>(nbFound - i))) * boost::lexical_cast<long>(groups[i]);
    }
  } else {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "Invalid walltime value: '" + walltime + "'");
  }

  return time;
}

/**
 * \brief Function to check the job status
 * \param status the status of the job
 * \return raises an exception on error
 */
void
vishnu::checkJobStatus(const int& status) {
  if ((status < -1) || (status > 8)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The status value is incorrect");
  }
}

/**
 * \brief Function to check the job priority
 * \param priority the priority of the job
 * \return raises an exception on error
 */
void
vishnu::checkJobPriority(const int& priority) {
  if ((priority < -1) || (priority > 5)) {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "The priority value is incorrect");
  }
}



/**
 * \brief Function to check the job nbNodesAndCpuPerNode
 * \param nbNodesAndNbCpuPerNode the number of nodes and cpu per node
 * \return raises an exception on error
 */
void
vishnu::checkJobNbNodesAndNbCpuPerNode(const std::string& nbNodesAndCpuPerNode) {
  if(nbNodesAndCpuPerNode.size()) {
    size_t posNbNodes;
    try {
      posNbNodes = nbNodesAndCpuPerNode.find(":");
      if(posNbNodes!=std::string::npos) {

        std::string nbNodes = nbNodesAndCpuPerNode.substr(0, posNbNodes);
        isNumericalValue(nbNodes);

        std::string cpuPerNode = nbNodesAndCpuPerNode.substr(posNbNodes+1);
        isNumericalValue(cpuPerNode);
      } else {
        throw UserException(ERRCODE_INVALID_PARAM,
                            ("Invalid NbNodesAndNbCpuPerNode value: "+nbNodesAndCpuPerNode));
      }
    } catch(UserException& ue) {
      throw UserException(ERRCODE_INVALID_PARAM,
                          ("Invalid NbNodesAndNbCpuPerNode value: "+nbNodesAndCpuPerNode));
    }
  }
}

/**
 * \brief Function to get the list of output files related to a job
 * \param result : The Job Result
 * \param appendJobId : Determine whether or not append the job id before the files lists
 * \return The list of files
 * Throw exception on error
 * */
//FIXME: do it with ssh and add try catch
std::string
vishnu::getResultFiles(const TMS_Data::JobResult & result,
                       const bool & appendJobId) {

  std::ostringstream existingFiles;
  if(appendJobId) {
    existingFiles << result.getJobId();
  }
  existingFiles << (existingFiles.str().size()? " " : "") << result.getOutputPath()
                << " " << result.getErrorPath()
                << " " << result.getOutputDir()
                << "\n";
  return existingFiles.str();
}


/**
 * \brief Function to create a directory
 * \param dirPath The path of the directory
 * Throw exception on error
 * */
void
vishnu::createOutputDir(std::string& dirPath) {

  if (bfs::exists(dirPath) && bfs::is_directory(dirPath)) {
    return ;
  }
  try{
    if (! bfs::create_directory(bfs::path(dirPath))) {
      throw SystemException(ERRCODE_SYSTEM, "vishnu::createDir: Cannot create the directory: " + dirPath) ;
    }
  } catch(...) {
    throw SystemException(ERRCODE_SYSTEM, "Directory creation failed") ;
  }
}


/**
 * \brief Function to create temporary file
 * \param fileName The name of the file to create
 * \param content The content of the file
 */
void
vishnu::saveInFile(const std::string & fileName,
                   const std::string& content) {

  std::ofstream file;
  file.open(fileName.c_str());
  if (!file.good()) {
    throw SystemException(ERRCODE_INVDATA,
                          "vishnu::saveInFile: Cannot create the file "+fileName);
  }
  file << content;
  file.close();
}

/**
 * \brief Function to create temporary file
 * \param fileName The name of the file to create
 * \param missingDesc The content of the file
 */
void
vishnu::recordMissingFiles(const std::string & fileName,
                           const std::string& missingDesc) {

  std::ofstream file;
  file.open(fileName.c_str());
  if (file.good()) {
    ListStrings missingFiles;
    boost::split(missingFiles, missingDesc, boost::is_space());
    int count = missingFiles.size();
    for(int i = 1; i < count; i++) {
      file << missingFiles[i] << "\n";
      //std::cout << missingFiles[i] << "\n";
    }
  }
  file.close();

}




/**
 * \brief To get the full path to the current executable
 */
// TODO: this method is not portable
// TODO: this method does not use boost
std::string
vishnu::getCurrentBinaryDir() {
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
    std::cerr << "Error resolving symlink /proc/self/exe.\n";
    exit(1);
  }
  if (length >= MAXPATHLEN) {
    std::cerr << "Path too long. Truncated.\n";
    exit(1);
  }
  
  /* I don't know why, but the string this readlink() function
   * returns is appended with a '@'.
   */
  fullpath[length] = '\0';       // Strip '@' off the end.
  
  // remove the executable name to get only the directory
  std::string execFullPath = fullpath;
  std::string execDir = execFullPath.substr(0,execFullPath.find_last_of("/"));
  
  return execDir;
}


/**
 * \brief Function to replace all occurences in a string
 * \param scriptContent The string to modify
 * \param oldValue The value to replace
 * \param newValue The new value
 */

void
vishnu::replaceAllOccurences(std::string& scriptContent,
                             const std::string& oldValue,
                             const std::string& newValue) {
  boost::replace_all(scriptContent, oldValue, newValue);
}

/**
 * \brief function to set parameters appearing in a script
 * \param scriptContent The string to modify
 * \param \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
 */
void
vishnu::setParams(std::string& scriptContent,
                  const std::string & params) {
  std::string paramName;
  std::string paramValue;
  size_t pos;
  ListStrings paramsVec;

  std::string& refParams = const_cast<std::string&>(params);
  boost::trim(refParams);
  boost::split(paramsVec, refParams, boost::is_any_of(" "));
  for (ListStrings::iterator it = paramsVec.begin(); it != paramsVec.end(); ++it) {
    pos = it->find("=");
    if (pos != std::string::npos) {
      paramName = it->substr(0, pos);
      paramValue = it->substr(pos+1, std::string::npos);
      replaceAllOccurences(scriptContent, "$" + paramName, paramValue);
      replaceAllOccurences(scriptContent, "${" + paramName + "}", paramValue);
    }
  }
}

/**
 * \brief Function to set environment variables according to parameters
 * \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
 */
void
vishnu::setParamsEnvVars(const std::string& params) {
  std::string param;
  size_t pos, pos1, pos2;

  pos1 = 0;
  while (pos2 = params.find(" ", pos1), pos2 != std::string::npos) {
    param = params.substr(pos1, pos2);
    pos = param.find("=");
    if (pos != std::string::npos) {
      setenv(param.substr(0, pos).c_str(),
             param.substr(pos + 1, std::string::npos).c_str(), 1);
    }
    pos1 = pos2 + 1;
  }
  param = params.substr(pos1, std::string::npos); //last token
  pos = param.find("=");
  if (pos != std::string::npos ) {
    setenv(param.substr(0, pos).c_str(),
           param.substr(pos+1, std::string::npos).c_str(), 1);
  }
}


/**
 * \brief Function to retrieve an environment variable
 * \param name The name of the variable
 * \param optional tell whether the parameter is optional or not
 * \param defaultValue give the default value return when the variable is optional
 * \return the value of the environment variable or throw exception is the variable is set and is not optional
 */
std::string
vishnu::getVar(const std::string& name,
               const bool & optional,
               const std::string defaultValue) {

  std::string value = defaultValue;
  char* cvalue = getenv(name.c_str());
  if (cvalue != NULL) {
    value = std::string(cvalue);
  } else if (!optional) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             "Missing parameter " +name);
  }

  return value;
}


/**
 * @brief Get the home directory of the currently logged user
 * @return  A string, throw exception on error
 */
std::string
vishnu::getCurrentUserHome(void)
{
  struct passwd *pw = getpwuid(getuid());
  if (! pw) {
    throw TMSVishnuException(ERRCODE_SYSTEM, boost::str(boost::format("getpwuid() failed with uid %1%") % getuid()));
  }
  return std::string(pw->pw_dir);
}


/**
 * @brief copy a given file to the current user home
 * @param path The file path
 * @return The resulting file path
 */
std::string
vishnu::copyFileToUserHome(const std::string& path)
{
  std::string result;
  try {
    bfs::path from(path);
    bfs::path to(boost::str(boost::format("%1%/%2%") % getCurrentUserHome() % bfs::basename(from)));
    bfs::copy_file(from, to, bfs::copy_option::overwrite_if_exists);
    result = to.native();
  } catch (const boost::filesystem::filesystem_error ex) {
    throw TMSVishnuException(ERRCODE_SYSTEM, ex.what());
  }

  return result;
}



/**
 * @brief Check a version of a given batch type is supported
 * @param btype The batch type
 * @param version The given version
 * @param supportedVersion If case of error return supported batch version
 * @return True on success, false otherwise
 */
bool
vishnu::checkIfSupportedBatchVersion(BatchType btype, const std::string& version, std::string& supportedVersion)
{
  std::map<int, std::set<std::string> >::const_iterator batchIter = SUPPORTED_BATCH_VERSIONS.find(btype);

  if (batchIter != SUPPORTED_BATCH_VERSIONS.end()) {
    std::set<std::string>::const_iterator versionIter = batchIter->second.find(version);
    if (versionIter != batchIter->second.end()) {
      return true;
    } else {
      std::ostringstream oss;
      std::copy(batchIter->second.begin(), batchIter->second.end(), std::ostream_iterator<std::string>(oss, ", "));
      supportedVersion = oss.str();
    }
  } else {
    supportedVersion = boost::str(boost::format("Unknown batch type %1%") % btype);
  }

  return false;
}
