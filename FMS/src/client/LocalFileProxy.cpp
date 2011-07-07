/**
 * \file LocalFileProxy.cpp
 * This file contains the definition of  proxy class to handle a local file
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \author Ibrahima Cisse (irahima.cisse@sysfera.com)
 */

#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>

#include "DIET_client.h"
#include "DIET_Dagda.h"

#include "SessionProxy.hpp"

#include "LocalFileProxy.hpp"
#include "RemoteFileProxy.hpp"
#include "FileProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "FMSVishnuException.hpp"
#include "boost/filesystem.hpp"
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace FMS_Data;
namespace bfs=boost::filesystem;
namespace ba=boost::algorithm;

/**
 * \brief Default constructor. 
 */
LocalFileProxy::LocalFileProxy() : FileProxy() {
  setHost("localhost");
  upToDate = false;
}

/* Standard constructor.
 * Use the file path as argument. */
LocalFileProxy::LocalFileProxy(const SessionProxy& sessionProxy, const string& path) : FileProxy(sessionProxy,path) {
  setHost("localhost");
  upToDate = false;
}

/* Copy constructor. */
LocalFileProxy::LocalFileProxy(const LocalFileProxy& file) : FileProxy(file) {
  upToDate = file.isUpToDate();
}

/* Copy operator. */
LocalFileProxy& LocalFileProxy::operator=(const LocalFileProxy& file) {
  FileProxy::operator=(file);
  upToDate = file.isUpToDate();
  return *this;
}

/* Return true if the file informations are up to date. */
bool LocalFileProxy::isUpToDate() const {
  return upToDate;
}

/* Obtain the information of a local file. Uses the POSIX standard functions. */
void LocalFileProxy::getInfos() const {
  file_stat_t fileStat;
  struct group* grp;
  struct passwd* usr;
  
  if (STAT(getPath().c_str(), &fileStat)==-1) {
    exists(false);
    return;
  }
  setPerms(fileStat.st_mode & ACCESSPERMS);
  setUid(fileStat.st_uid);
  setGid(fileStat.st_gid);
  setSize(fileStat.st_size);
  setAtime(fileStat.st_atime);
  setMtime(fileStat.st_mtime);
  setCtime(fileStat.st_ctime);
 
  if (S_ISBLK(fileStat.st_mode)){
    setType(0); //BLOCK
  }
  if (S_ISCHR(fileStat.st_mode)){
    setType(1); // CHARACTER
  }
 
  if (S_ISDIR(fileStat.st_mode)){
    setType(2);//DIRECTORY
  }
 
  if (S_ISLNK(fileStat.st_mode)){
    setType(3);//SYMBOLICLINK
  }
  if (S_ISSOCK(fileStat.st_mode)){
    setType(4);//SCKT
  }
  if (S_ISFIFO(fileStat.st_mode)){
    setType(5);//FIFO
  }
  
  if (S_ISREG(fileStat.st_mode)){
    setType(6);//REGULAR
  }
  
  usr = getpwuid(fileStat.st_uid);
  grp = getgrgid(fileStat.st_gid);
  
  if (usr!=NULL){
    setOwner(usr->pw_name);
  }
  if (grp!=NULL){
    setGroup(grp->gr_name);
  }
  
  exists(true);
  upToDate=true;
}


/* Copy the local file to remote destination. */
/* The function proceed to the file copy by itself if the
 * destination is a local path. Otherwise it calls the DIET service.
 */
template <class TypeOfOption>
int LocalFileProxy::transferFile(const string& dest, 
                                 const TypeOfOption& options, 
                                 const std::string& serviceName,
                                 FileTransfer& fileTransfer) {

  string host = FileProxy::extHost(dest);
  std::cout << "dest " << host << "\n";
  std::cout << "host " << host << "\n";
  bfs::path localFullPath(bfs::current_path().string());
  
  // get the source full qualified host name 

  std::string srcHost(vishnu::getLocalMachineName("22"));

  uid_t uid = getuid();
  struct passwd*  pw = getpwuid(uid);
  char* localUser = pw->pw_name;
  
  if(getPath().compare(".")!=0){
    localFullPath/= getPath(); 
  }


  char *optionsToString = NULL; 
  char *fileTransferInString = NULL;

  if (host=="localhost") {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, "The local to local transfer is not available");
  } 

  diet_profile_t* profile;
  char* errMsg;
  
  std::string sessionKey=this->getSession().getSessionKey();

  bool isAsyncTransfer = (serviceName.compare("FileCopyAsync")==0 || serviceName.compare("FileMoveAsync")==0);
  if(!isAsyncTransfer) { 
    profile = diet_profile_alloc(const_cast<char*>(serviceName.c_str()), 5, 5, 6);
  } else {
    profile = diet_profile_alloc(const_cast<char*>(serviceName.c_str()), 5, 5, 7);
  }
  

  if (ba::starts_with(getPath(),"/")  ){

    localFullPath=getPath();

  }


  //IN Parameters  
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localFullPath.string().c_str()),
      DIET_VOLATILE); // local source file
  diet_string_set(diet_parameter(profile, 2), localUser, DIET_VOLATILE);
 
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(srcHost.c_str()),
      DIET_VOLATILE);


  diet_string_set(diet_parameter(profile, 4), const_cast<char*>(dest.c_str()), DIET_VOLATILE); 

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<TypeOfOption*>(&options)).c_str());

  diet_string_set(diet_parameter(profile,5 ), optionsToString, DIET_VOLATILE);

  if(!isAsyncTransfer) {
    diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  } else {
    diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile, 7), NULL, DIET_VOLATILE); 
  }

  if (diet_call(profile)) {
    raiseDietMsgException("Error calling DIET service");
  }

  if(!isAsyncTransfer) {
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);
  
  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);
  } else {
    diet_string_get(diet_parameter(profile, 6), &fileTransferInString, NULL);
    diet_string_get(diet_parameter(profile, 7), &errMsg, NULL);

    /*To raise a vishnu exception if the received message is not empty*/
    raiseExceptionIfNotEmptyMsg(errMsg);

    FMS_Data::FileTransfer_ptr fileTransfer_ptr = NULL;

    parseEmfObject(std::string(fileTransferInString), fileTransfer_ptr);

    fileTransfer = *fileTransfer_ptr;

  }

  return 0;
}

int LocalFileProxy::cp(const string& dest, const CpFileOptions& options) {
 FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
 return transferFile(dest, options, "FileCopy", fileTransfer);

}

int LocalFileProxy::mv(const string& dest, const CpFileOptions& options) {
 FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
 return  transferFile(dest, options, "FileMove", fileTransfer);

}

int LocalFileProxy::cpAsync(const std::string& dest, const CpFileOptions& options, FileTransfer& fileTransfer) {
  return transferFile(dest, options, "FileCopyAsync", fileTransfer);
}

int LocalFileProxy::mvAsync(const std::string& dest, const CpFileOptions& options, FileTransfer& fileTransfer) {
  return transferFile(dest, options, "FileMoveAsync", fileTransfer);
}

