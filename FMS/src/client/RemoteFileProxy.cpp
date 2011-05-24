#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/types.h>

#include "DIET_data.h"
#include "DIET_client.h"
#include "DIET_Dagda.h"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "RemoteFileProxy.hpp"
#include "LocalFileProxy.hpp"

using namespace std;
using namespace FMS_Data;

/* Default constructor. */
RemoteFileProxy::RemoteFileProxy() {
  upToDate = false;
}

/* Standard constructor.
 * Take the file path and user name as parameters.
 */
RemoteFileProxy::RemoteFileProxy(const SessionProxy& sessionProxy,const string& path,
                       const string& localUser) : FileProxy(sessionProxy,path)
{
  upToDate = false;
  this->localUser = localUser;
}

/* Copy constructor. */
RemoteFileProxy::RemoteFileProxy(const RemoteFileProxy& file) : FileProxy(file) {
  upToDate = false;
  this->localUser = file.localUser;
}

/* Standard destructor. */
RemoteFileProxy::~RemoteFileProxy() {
}

/* Returns true if the file informations are up to date. */
bool RemoteFileProxy::isUpToDate() const {
  return upToDate;
}

/* Get the informations about this remote file. Call the DIET service. */
void RemoteFileProxy::getInfos() const {
  
  diet_profile_t* profile;
  
  char *fileStatInString=NULL, * errMsg=NULL;
  
  std::string serviceName("FileGetInfos");
 
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);

  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
 
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to obtain file informations");


  diet_string_get(diet_parameter(profile, 4), &fileStatInString, NULL);


  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);
  
  if (strlen(fileStatInString )!=0){
    std::cout << "fileStatInString"<< fileStatInString <<"\n";
    FileStat_ptr fileStat_ptr = NULL;
    parseEmfObject(std::string(fileStatInString), fileStat_ptr);
    setFileStat(*fileStat_ptr);
    exists(true);
    upToDate = true;
    delete fileStat_ptr;
  }

else{
  exists(false);
  raiseExceptionIfNotEmptyMsg(errMsg);
}

}

/* Copy operator. */
RemoteFileProxy& RemoteFileProxy::operator=(const RemoteFileProxy& file) {
  FileProxy::operator=(file);
  localUser = file.localUser;
  upToDate = file.isUpToDate();
  return *this;
}

/* Call the change group DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::chgrp(const string& group) {
  diet_profile_t* profile;
  char* errMsg;
 
  std::string serviceName("FileChangeGroup");
 
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), const_cast<char*>(group.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);

  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to change file group");
  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);

  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }
  setGroup(group);
  return 0;
}

/* Call the change mode DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::chmod(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
 
  std::string serviceName("FileChangeMode");
 
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE);

  ostringstream os;
  os << mode;
  string modeInString (os.str());

  diet_string_set(diet_parameter(profile, 4), const_cast<char*>(modeInString.c_str()),
                                            DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);


  if (diet_call(profile)){
    throw runtime_error("Error calling DIET service to change file mode");
  }


  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }
  setPerms(mode);

  return 0;
}

/* Call the file head DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
string RemoteFileProxy::head(const HeadOfFileOptions& options) {
  string result;
  char* fileHead, * errMsg, *optionsToString = NULL;
  diet_profile_t* profile;
  
  std::string serviceName("FileHead");
 
  std::string sessionKey=this->getSession().getSessionKey();
  

  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
 
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
 
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);

  const char* name = "head";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options)).c_str());


  diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");

  diet_string_get(diet_parameter(profile, 5), &fileHead, NULL);
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);

  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }
  
  result = fileHead;
  return result;
}

/* Call the file getContent DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
string RemoteFileProxy::getContent() {
  string result;
  char* fileContent, * errMsg;
  diet_profile_t* profile;
  
  std::string serviceName("FileContent");
 
  std::string sessionKey=this->getSession().getSessionKey();
  
  

  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
 
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
 
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);


  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");

  diet_string_get(diet_parameter(profile, 4), &fileContent, NULL);
  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);

  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }
  
  
  return fileContent;
}

/* Call the mkfile DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::mkfile(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  std::string serviceName("FileCreate");
  
  std::string sessionKey=this->getSession().getSessionKey();
  
  
  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  
      
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
    diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
  raiseExceptionIfNotEmptyMsg(errMsg);
  }  
  
  exists(true);
  
  return 0;  
}

/* Call the mkdir DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::mkdir(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  std::string serviceName("DirCreate");
  
  std::string sessionKey=this->getSession().getSessionKey();


 profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  
      
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
    diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
  raiseExceptionIfNotEmptyMsg(errMsg);
  }  
  
  exists(true);
  
  return 0;  
}


/* Call the remove file DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::rm() {
  diet_profile_t* profile;
  char* errMsg;
  
  std::string serviceName("FileRemove");
  
  std::string sessionKey=this->getSession().getSessionKey();


 profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  
      
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }  
  
  exists(true);
  return 0;
}

/* Call the remove directory DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::rmdir() {
  diet_profile_t* profile;
  char* errMsg;
  
  std::string serviceName("DirRemove");
  
  std::string sessionKey=this->getSession().getSessionKey();


 profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  
      
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    raiseExceptionIfNotEmptyMsg(errMsg);
  }  
  
  exists(true);
  return 0;
 

}

/* Call the file tail DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
string RemoteFileProxy::tail(const TailOfFileOptions& options) {
  string result;
  char* fileTail, * errMsg, *optionsToString = NULL;
  diet_profile_t* profile;
  
  std::string serviceName("FileTail");
  std::string sessionKey=this->getSession().getSessionKey();
  
  
  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
  
  diet_string_set(diet_parameter(profile, 0), strdup(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
 
  const char* name = "tail";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options)).c_str()); 
  
  diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 5), &fileTail, NULL);
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
  raiseExceptionIfNotEmptyMsg(errMsg);
  }
  result = fileTail;
  return result;
}

/* Call the ls DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */

list<string> RemoteFileProxy::ls(const LsDirOptions& options) const {
 
 list<string> result;
  char* errMsg, *ls, *optionsToString = NULL;
  diet_profile_t* profile;

  std::string serviceName("DirList");
  std::string sessionKey=this->getSession().getSessionKey();
  
  
  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
  
  diet_string_set(diet_parameter(profile, 0), strdup(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);

  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
 
  const char* name = "ls";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize(const_cast<FMS_Data::LsDirOptions_ptr>(&options)).c_str()); 
  
  diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE);
  
  
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 5), &ls, NULL);
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
  raiseExceptionIfNotEmptyMsg(errMsg);
  }

// post traitemennt 
  istringstream is(ls);
  char buffer[1024];
  string line;

  while (!is.eof()) {
    is.getline(buffer, 1024);
    line = buffer;
    result.push_back(line);
  }

  return result;
}





void RemoteFileProxy::printTransferID(const bool printTrID) {
  this->printTrID=printTrID;
}
