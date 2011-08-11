/**
 * \file SSHJobExec.cpp
 * \brief This file presents the implementation of the SSHJobExec.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SSHJobExec.hpp"

const std::string TMS_SERVER_FILES_DIR="/tmp";

/**
 * \brief Constructor
 * \param user the user login
 * \param hostname the hostname of the machine
 * \param batchType the type of the batch scheduler
 * \param jobSerialized the job serialized
 * \param submitOptionsSerialized the job options serialized
 */
SSHJobExec::SSHJobExec(const std::string& user,
                       const std::string& hostname,
                       const BatchType& batchType,
                       const std::string& jobSerialized,
                       const std::string& submitOptionsSerialized):
 muser(user), mhostname(hostname), mbatchType(batchType), mjobSerialized(jobSerialized),
 msubmitOptionsSerialized(submitOptionsSerialized)
{
}

/**
 * \brief Function to return the job serialized content
 * \return  job serialized content
 */
std::string
SSHJobExec::getJobSerialized() {
 return mjobSerialized;
}

/**
 * \brief Function to return the error message of a service
 * \return error message information
 */
std::string
SSHJobExec::getErrorInfo() {
 return merrorInfo;
}

/**
 * \brief Function to check the parameters before launching ssh
 * \return raises an exception on error
 */
void
SSHJobExec::checkSshParams() {
  if (muser.empty()) {
    throw SystemException(ERRCODE_SSH, "User login is empty");
  }
  if (mhostname.empty()) {
    throw SystemException(ERRCODE_SSH, "Server hostname is empty");
  }
}


/**
 * \brief Function to execute command by using ssh
 * \param slaveDirectory the path to the command executable
 * \param serviceName the name of the service to execute
 * \param script_path the path to script to submit
 * \return raises an exception on error
 */
void
SSHJobExec::sshexec(const std::string& slaveDirectory,
                        const std::string& serviceName,
                        const std::string& script_path) {

  checkSshParams();

  std::string jobSerializedPath;
  std::string submitOptionsSerializedPath;
  std::string jobUpdateSerializedPath;
  std::string errorPath;
  std::string stderrFilePath;
  bool wellSubmitted = false;

  jobSerializedPath = TMS_SERVER_FILES_DIR+"/jobSerializedXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(jobSerializedPath.c_str()), mjobSerialized);

  submitOptionsSerializedPath = TMS_SERVER_FILES_DIR+"/submitOptionsSerializedXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(submitOptionsSerializedPath.c_str()), msubmitOptionsSerialized);

  jobUpdateSerializedPath = TMS_SERVER_FILES_DIR+"/jobUpdateSerializedXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(jobUpdateSerializedPath.c_str()));

  errorPath = TMS_SERVER_FILES_DIR+"/errorPathXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(errorPath.c_str()));

  std::ostringstream cmd;
  cmd << "ssh -l " << muser << " " << mhostname << " ";
  cmd << " -o NoHostAuthenticationForLocalhost=yes ";
  cmd << " -o PasswordAuthentication=no ";
  cmd << slaveDirectory << "/tmsSlave ";
  cmd << serviceName << " ";
  cmd << convertBatchTypeToString(mbatchType) << " ";
  cmd << jobSerializedPath << " " <<  errorPath << " ";
  if(serviceName.compare("SUBMIT")==0) {
    cmd <<  jobUpdateSerializedPath << " " <<  submitOptionsSerializedPath;
    cmd << " " << script_path;
  }

  stderrFilePath = TMS_SERVER_FILES_DIR+"/stderrFilePathXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(stderrFilePath.c_str()));
  cmd << " 2> " << stderrFilePath;
  int ret;
  if((ret=system((cmd.str()).c_str()))) {
    vishnu::deleteFile(jobSerializedPath.c_str());
    vishnu::deleteFile(submitOptionsSerializedPath.c_str());
    vishnu::deleteFile(jobUpdateSerializedPath.c_str());
    vishnu::deleteFile(errorPath.c_str());
    boost::filesystem::path stderrFile(stderrFilePath.c_str());
    if(!boost::filesystem::is_empty(stderrFile)) {
      merrorInfo = vishnu::get_file_content(stderrFilePath);
      if(merrorInfo.find("password")!=std::string::npos) {
        merrorInfo.append("  You must copy the VISHNU publickey in your authorized_keys file.");
      }
    }
    if((WEXITSTATUS(ret)==1)&&(mbatchType==SLURM)) {//ATTENTION: 1 corresponds of the error_exit value in ../slurm_parser/opt.c
      merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+merrorInfo);
    }
    vishnu::deleteFile(stderrFilePath.c_str());
    throw SystemException(ERRCODE_SSH, merrorInfo);
  }

  boost::filesystem::path jobUpdateSerializedFile(jobUpdateSerializedPath);
  if(!boost::filesystem::is_empty(jobUpdateSerializedFile)) {
    std::string jobSerialized = vishnu::get_file_content(jobUpdateSerializedPath);
    TMS_Data::Job_ptr job = NULL;
    if(!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
      vishnu::deleteFile(jobSerializedPath.c_str());
      vishnu::deleteFile(submitOptionsSerializedPath.c_str());
      vishnu::deleteFile(jobUpdateSerializedPath.c_str());
      vishnu::deleteFile(errorPath.c_str());
      vishnu::deleteFile(stderrFilePath.c_str());
      throw SystemException(ERRCODE_INVDATA, "SSHJobExec::sshexec: job object is not well built");
    }
    ::ecorecpp::serializer::serializer _ser;//("job");
    mjobSerialized = strdup(_ser.serialize_str(job).c_str());
    wellSubmitted = true;
    delete job;
  }

  boost::filesystem::path errorFile(errorPath.c_str());
  if(!boost::filesystem::is_empty(errorFile)) {
    merrorInfo = vishnu::get_file_content(errorPath);
    merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
  }

  if((mbatchType==LOADLEVELER) && (wellSubmitted==false)) {
    boost::filesystem::path stderrFile(stderrFilePath.c_str());
    if(!boost::filesystem::is_empty(stderrFile)) {
      merrorInfo = vishnu::get_file_content(stderrFilePath);

      std::ostringstream errorMsgSerialized;
      errorMsgSerialized << ERRCODE_BATCH_SCHEDULER_ERROR << "#" << "LOADLEVELER ERROR: ";
      errorMsgSerialized << merrorInfo;
      merrorInfo = errorMsgSerialized.str();
      merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
    }
  }


  vishnu::deleteFile(jobSerializedPath.c_str());
  vishnu::deleteFile(submitOptionsSerializedPath.c_str());
  vishnu::deleteFile(jobUpdateSerializedPath.c_str());
  vishnu::deleteFile(errorPath.c_str());
  vishnu::deleteFile(stderrFilePath.c_str());
}

/**
 * \brief Function to convert the batch type to string
 * \param BatchType the batch type to convert
 * \return the converted batch type
 */
std::string SSHJobExec::convertBatchTypeToString(BatchType batchType) {
  std::string value;
  switch(batchType) {
    case TORQUE:
      value = "TORQUE";
      break;
    case LOADLEVELER:
      value = "LOADLEVELER";
      break;
    case SLURM:
      value = "SLURM";
      break;
    default:
      value = "UNKNOWN_BATCH_TYPE";
      break;
  }
  return value;
}

/**
 * \brief Function to copy files from remote machine
 * \param outputPath the output path to get
 * \param errorPath the error path to get
 * \param copyOfOutputPath the copy of the outputPath
 * \param copyOfErrorPath the copy of errorPath
 * \return raises an exception on error
 */
int
SSHJobExec::copyFiles(const std::string& outputPath,
                      const std::string& errorPath,
                      const char* copyOfOutputPath,
                      const char* copyOfErrorPath) {

  std::ostringstream cmd1;
  cmd1 << "scp -o NoHostAuthenticationForLocalhost=yes  -o PasswordAuthentication=no ";
  cmd1 << muser << "@" << mhostname << ":" << outputPath << " " << copyOfOutputPath;
  if(system((cmd1.str()).c_str())) {
    return -1;
  }

  std::ostringstream cmd2;
  cmd2 << "scp -o NoHostAuthenticationForLocalhost=yes  -o PasswordAuthentication=no ";
  cmd2 << muser << "@" << mhostname << ":" << errorPath << " " << copyOfErrorPath;
  if(system((cmd2.str()).c_str())) {
    return -1;
  }
 return 0;
}

/**
 * \brief Destructor
 */
SSHJobExec::~SSHJobExec() {

}
