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
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <fstream>

/**
 * \brief Function to convert a string to a batch type
 * \param  batchName the name of the batch type
 * \return BatchType value of the corresponding string.
 */
BatchType
vishnu::convertToBatchType(const std::string& batchName) {

  BatchType batchType = UNDEFINED;
  if (batchName == "TORQUE") {
    batchType = TORQUE;
  } else if (batchName == "LOADLEVELER") {
    batchType = LOADLEVELER;
  } else if (batchName == "SLURM") {
    batchType = SLURM;
  } else if (batchName == "LSF") {
    batchType = LSF;
  } else if (batchName == "SGE") {
    batchType = SGE;
  } else if (batchName == "DELTACLOUD") {
    batchType = DELTACLOUD;
  } else if (batchName == "PBS") {
    batchType = PBSPRO;
  } else if (batchName == "POSIX") {
    batchType = POSIX;
  }

  return batchType;
}

/**
 * \brief  function to convert job status into string
 * \param state: The state of job
 * \return The converted state value
 */
std::string
vishnu::convertJobStateToString(const int& state) {

  std::string stateStr;
  switch(state) {
  case vishnu::STATE_SUBMITTED:
    stateStr = "SUBMITTED";
    break;
  case vishnu::STATE_QUEUED:
    stateStr = "QUEUED";
    break;
  case vishnu::STATE_WAITING:
    stateStr = "WAITING";
    break;
  case vishnu::STATE_RUNNING:
    stateStr = "RUNNING";
    break;
  case vishnu::STATE_COMPLETED:
    stateStr = "COMPLETED";
    break;
  case vishnu::STATE_CANCELLED:
    stateStr = "CANCELLED";
    break;
  case vishnu::STATE_DOWNLOADED:
    stateStr = "DOWNLOADED";
    break;
  case vishnu::STATE_FAILED:
    stateStr = "FAILED";
    break;
  case vishnu::STATE_UNDEFINED:
  default:
    stateStr = "UNDEFINED";
    break;
  }
  return stateStr;
}

/**
 * \brief Function a given walltime into string
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

  long time = 0;

  if (boost::regex_match(walltime, groups, reg)) {
    boost::smatch::const_iterator it;
    unsigned int start = 1;  // skip first value which is the full string

    it = groups.begin();
    ++it;  // skip first value which is the full string
    /* Count the number of not empty groups (-1, as the first element
     * in the group is the full string)
     */
    unsigned int nbFound =
      groups.size() - 1 - std::count(it, groups.end(), "");

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
