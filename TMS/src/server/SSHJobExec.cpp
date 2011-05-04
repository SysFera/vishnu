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

SSHJobExec::SSHJobExec() {
  mscript_path = NULL;
  mjobSerialized = "";
  msubmitOptionsSerialized = "";
  mbatchType = UNDEFINED;
  merrorInfo = "";
  muser = "";
  mhostname = "";
  msshKey = "";
}

SSHJobExec::SSHJobExec(char* script_path,
                       const std::string& jobSerialized,
                       const std::string& submitOptionsSerialized,
                       const std::string& user,
                       const std::string& hostname,
                       const std::string& sshKey,
                       BatchType batchType)
{
  mscript_path = script_path;
  mjobSerialized = jobSerialized;
  msubmitOptionsSerialized = submitOptionsSerialized;
  mbatchType = batchType;
  merrorInfo = "";
  muser = user;
  mhostname = hostname;
  msshKey = sshKey;
}

std::string SSHJobExec::getJobSerialized() {
 return mjobSerialized;
}

std::string SSHJobExec::getErrorInfo() {
 return merrorInfo;
}

int SSHJobExec::sshexec(const std::string& slaveDirectory, 
                        const std::string& action) {

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
  cmd << slaveDirectory << "/tmsSlave ";
  cmd << action << " ";
  cmd << convertBatchTypeToString(mbatchType) << " ";
  cmd << jobSerializedPath << " " <<  errorPath << " ";
  if(action.compare("SUBMIT")==0) {
    cmd <<  jobUpdateSerializedPath << " " <<  submitOptionsSerializedPath;
    cmd << " " << mscript_path;
  }

  stderrFilePath = TMS_SERVER_FILES_DIR+"/stderrFilePathXXXXXX";
  vishnu::createTmpFile(const_cast<char*>(stderrFilePath.c_str()));
  cmd << " 2> " << stderrFilePath;

  std::cout << cmd.str() << std::endl;
  if(system((cmd.str()).c_str())) { //A REMPLACER PAR exec
    std::cerr << "can't execute " << cmd.str() << std::endl;
    vishnu::deleteFile(jobSerializedPath.c_str());
    vishnu::deleteFile(submitOptionsSerializedPath.c_str());
    vishnu::deleteFile(jobUpdateSerializedPath.c_str());
    vishnu::deleteFile(errorPath.c_str());
    boost::filesystem::path stderrFile(stderrFilePath.c_str());
    if(!boost::filesystem::is_empty(stderrFile)) {
      std::string stderrMsg = "SSHJobExec::sshexec: can't execute tmsSlave executable with ssh -l, the error are: \n";
      merrorInfo = vishnu::get_file_content(stderrFilePath);
      stderrMsg.append(merrorInfo);
      merrorInfo = stderrMsg;
    }
    vishnu::deleteFile(stderrFilePath.c_str());
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, merrorInfo);
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
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "SSHJobExec::sshexec: job object is not well built");
    }
    ::ecorecpp::serializer::serializer _ser("job");
    mjobSerialized = strdup(_ser.serialize(job).c_str());
    wellSubmitted = true;
    delete job;
  }

  boost::filesystem::path errorFile(errorPath.c_str());
  if(!boost::filesystem::is_empty(errorFile)) {
    merrorInfo = vishnu::get_file_content(errorPath);
    merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
    std::cout << "merrorInfo = " << merrorInfo << std::endl;
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
      std::cout << "merrorInfo = " << merrorInfo << std::endl;
    }
  }


  vishnu::deleteFile(jobSerializedPath.c_str());
  vishnu::deleteFile(submitOptionsSerializedPath.c_str());
  vishnu::deleteFile(jobUpdateSerializedPath.c_str());
  vishnu::deleteFile(errorPath.c_str());
  vishnu::deleteFile(stderrFilePath.c_str());
}

std::string SSHJobExec::convertBatchTypeToString(BatchType batchType) {

  switch(batchType) {
    case TORQUE:
      return "TORQUE";
    case LOADLEVELER:
      return "LOADLEVELER";
    default:
      return "";
  }
}

int SSHJobExec::copyFiles(const std::string& outputPath, 
                          const std::string& errorPath,
                          const char* copyOfOutputPath, 
                          const char* copyOfErrorPath) {

  std::ostringstream cmd1;
  cmd1 << "scp " << muser << "@" << mhostname << ":" << outputPath << " " << copyOfOutputPath;
  if(system((cmd1.str()).c_str())) { //A REMPLACER PAR exec
    return -1;
  }

  std::ostringstream cmd2;
  cmd2 << "scp " << muser << "@" << mhostname << ":" << errorPath << " " << copyOfErrorPath;
  if(system((cmd2.str()).c_str())) { //A REMPLACER PAR exec
    return -1;
  }
 return 0;
}

SSHJobExec::~SSHJobExec() {

}
