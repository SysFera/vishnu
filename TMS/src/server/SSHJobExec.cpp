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
#include "emfTMSUtils.hpp"
#include "utilVishnu.hpp"
#include "SSHJobExec.hpp"

const std::string TMS_SERVER_FILES_DIR="/tmp";
const std::string DEFAULT_SLAVE_EXECUTABLE_DIR = "/tmp";

SSHJobExec::SSHJobExec() {
  mscript_path = NULL;
  mjobSerialized = "";
  msubmitOptionsSerialized = "";
  mbatchType = UNDEFINED;
  merrorInfo = "";
}

SSHJobExec::SSHJobExec(char* script_path, 
                       const std::string& jobSerialized, 
                       const std::string& submitOptionsSerialized,
                       BatchType batchType) 
{
  mscript_path = script_path;
  mjobSerialized = jobSerialized;
  msubmitOptionsSerialized = submitOptionsSerialized;
  mbatchType = batchType;
  merrorInfo = "";
}
     
std::string SSHJobExec::getJobSerialized() {
 return mjobSerialized;
}

std::string SSHJobExec::getErrorInfo() {
 return merrorInfo;
}

void SSHJobExec::createTmpFile(char* fileName, const std::string& file_content) {

  int  file_descriptor = mkstemp( fileName ) ;
  size_t file_size = file_content.size();
  if( file_descriptor == -1 ) {
    std::cerr << "SSHJobExec: Cannot create new tmp file" << std::endl;
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )!=0) {
     std::cerr << "SSHJobExec: WARNING: reading or writing rights have not been change on the path. This can lead to an error" << std::endl;
  }

  if( write(file_descriptor, file_content.c_str(), file_size) != file_size ) {
    std::cerr << "SSHJobExec: Cannot write the content int to new created file" << std::endl;
  }

  close(file_descriptor);
}

void SSHJobExec::createTmpFile(char* fileName) {

  int  file_descriptor = mkstemp( fileName ) ;
  if( file_descriptor == -1 ) {
    std::cerr << "SSHJobExec: Cannot create new tmp file" << std::endl;
  }

  if(fchmod(file_descriptor, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) !=0) {
     std::cerr << "SSHJobExec: WARNING: reading or writing rights rights have not been set on the path. This can lead to an error" << std::endl;
  }

  close(file_descriptor);
}

int SSHJobExec::deleteFile(const char* fileName) {
  return unlink(fileName); 
}

int SSHJobExec::sshexec(const std::string& action) {

  std::string jobSerializedPath;
  std::string submitOptionsSerializedPath;
  std::string jobUpdateSerializedPath;
  std::string errorPath;

  try {
    jobSerializedPath = TMS_SERVER_FILES_DIR+"/jobSerialized.XXXXXX";
    createTmpFile(const_cast<char*>(jobSerializedPath.c_str()), mjobSerialized);

    submitOptionsSerializedPath = TMS_SERVER_FILES_DIR+"/submitOptionsSerialized.XXXXXX";
    createTmpFile(const_cast<char*>(submitOptionsSerializedPath.c_str()), msubmitOptionsSerialized);

    jobUpdateSerializedPath = TMS_SERVER_FILES_DIR+"/jobUpdateSerialized.XXXXXX";
    createTmpFile(const_cast<char*>(jobUpdateSerializedPath.c_str()));

    errorPath = TMS_SERVER_FILES_DIR+"/errorPath.XXXXXX";
    createTmpFile(const_cast<char*>(errorPath.c_str()));

    std::ostringstream cmd;
    //cmd << "ssh -l traore localhost ";
    cmd << "ssh localhost ";
    cmd << DEFAULT_SLAVE_EXECUTABLE_DIR << "/tmsSlave ";
    cmd << action << " ";
    cmd << jobSerializedPath << " " <<  errorPath << " ";
    if(action.compare("SUBMIT")==0) {
      cmd <<  jobUpdateSerializedPath << " " <<  submitOptionsSerializedPath;
      cmd << " " << mscript_path;
    }

    if(system((cmd.str()).c_str())) {
      std::cerr << "can't execute " << cmd.str() << std::endl;
    }

    boost::filesystem::path jobUpdateSerializedFile(jobUpdateSerializedPath);
    if(!boost::filesystem::is_empty(jobUpdateSerializedFile)) {
      std::string jobSerialized = vishnu::get_file_content(jobUpdateSerializedPath);
      TMS_Data::Job_ptr job = NULL;
      if(!vishnu::parseTMSEmfObject(std::string(jobSerialized), job)) {
        std::cout << "parseEmfObject returns NULL...." << std::endl;
        return 1;
      }
      ::ecorecpp::serializer::serializer _ser("job");
      mjobSerialized = strdup(_ser.serialize(job).c_str());
      delete job;
    }

    boost::filesystem::path errorFile(errorPath.c_str());
    if(!boost::filesystem::is_empty(errorFile)) {
      merrorInfo = vishnu::get_file_content(errorPath);
      merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
      std::cout << "merrorInfo = " << merrorInfo << std::endl;
    }

    deleteFile(jobSerializedPath.c_str());
    deleteFile(submitOptionsSerializedPath.c_str());
    deleteFile(jobUpdateSerializedPath.c_str());
    deleteFile(errorPath.c_str());

  } catch(std::exception& exec) {
    deleteFile(jobSerializedPath.c_str());
    deleteFile(submitOptionsSerializedPath.c_str());
    deleteFile(jobUpdateSerializedPath.c_str());
    deleteFile(errorPath.c_str());
    std::cerr << exec.what() << std::endl;
  }
}

SSHJobExec::~SSHJobExec() {

}
