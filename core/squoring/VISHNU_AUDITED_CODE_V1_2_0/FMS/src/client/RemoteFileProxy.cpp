#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <pwd.h>

#include "DIET_data.h"
#include "DIET_client.h"
#include "DIET_Dagda.h"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "RemoteFileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "boost/filesystem.hpp"
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace FMS_Data;
namespace bfs=boost::filesystem;
namespace ba=boost::algorithm;

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

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){  
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);

  if (diet_call(profile)) {
    raiseDietMsgException("Error calling DIET service to obtain file informations");
  }

  if(diet_string_get(diet_parameter(profile, 4), &fileStatInString, NULL)){
    msgErrorDiet += " by receiving FileStat serialized  content";
    raiseDietMsgException(msgErrorDiet);
  }


  if(diet_string_get(diet_parameter(profile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  if (strlen(fileStatInString )!=0){
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
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::chgrp(const string& group) {
  diet_profile_t* profile;
  char* errMsg;

  std::string serviceName("FileChangeGroup");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet); 
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 4), const_cast<char*>(group.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with group parameter "+group;
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);

  if (diet_call(profile)) {
    raiseDietMsgException("Error calling DIET service to change file group");
  }

  if(diet_string_get(diet_parameter(profile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);
  setGroup(group);

  return 0;
}

/* Call the change mode DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::chmod(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;

  std::string serviceName("FileChangeMode");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ostringstream os;
  os << mode;
  string modeInString (os.str());

  if(diet_string_set(diet_parameter(profile, 4), const_cast<char*>(modeInString.c_str()),
      DIET_VOLATILE)){
    msgErrorDiet += " by receiving mode parameter "+modeInString;
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);


  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service to obtain file informations");
  }

  //Output parameter
  if(diet_string_get(diet_parameter(profile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  setPerms(mode);

  return 0;
}

/* Call the file head DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
string RemoteFileProxy::head(const HeadOfFileOptions& options) {
  string result;
  char* fileHead, * errMsg, *optionsToString = NULL;
  diet_profile_t* profile;

  std::string serviceName("FileHead");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters  
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options)).c_str());


  if(diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE)){
    msgErrorDiet += " by receiving head of file options values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }
  
  //Output parameter
  if(diet_string_get(diet_parameter(profile, 5), &fileHead, NULL)){
    msgErrorDiet += " by receiving fileHead message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(profile, 6), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  result = fileHead;
  return result;
}

/* Call the file getContent DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
string RemoteFileProxy::getContent() {
  string result;
  char* fileContent, * errMsg;
  diet_profile_t* profile;

  std::string serviceName("FileContent");

  std::string sessionKey=this->getSession().getSessionKey();

  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile, 4), &fileContent, NULL)){
    msgErrorDiet += " by receiving fileContent message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(profile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);


  return fileContent;
}

/* Call the mkfile DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::mkfile(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  std::string serviceName("FileCreate");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile, 4), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);

  return 0;  
}

/* Call the mkdir DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::mkdir(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  std::string serviceName("DirCreate");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile, 4), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);

  return 0;  
}


/* Call the remove file DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::rm(const RmFileOptions& options) {
  diet_profile_t* profile;
  char* errMsg;
 char* optionsToString=NULL;
  std::string serviceName("FileRemove");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<RmFileOptions*>(&options)).c_str());
  
  diet_string_set(diet_parameter(profile,4 ), optionsToString, DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile,5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);
  return 0;
}

/* Call the remove directory DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::rmdir() {
  diet_profile_t* profile;
  char* errMsg;

  std::string serviceName("DirRemove");

  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters    
  if(diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile, 4), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/ 
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);
  return 0;


}

/* Call the file tail DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
string RemoteFileProxy::tail(const TailOfFileOptions& options) {
  string result;
  char* fileTail, * errMsg, *optionsToString = NULL;
  diet_profile_t* profile;

  std::string serviceName("FileTail");
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), strdup(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options)).c_str()); 

  if(diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE)){
    msgErrorDiet += " by receiving tail of file options values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);

  if (diet_call(profile)){
    raiseDietMsgException("Error calling DIET service");
  }

  if(diet_string_get(diet_parameter(profile, 5), &fileTail, NULL)){
     msgErrorDiet += " by receiving fileTail message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(profile, 6), &errMsg, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  result = fileTail;
  return result;
}

/* Call the ls DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */

list<string> RemoteFileProxy::ls(const LsDirOptions& options) const {

  list<string> result;
  char* errMsg, *ls, *optionsToString = NULL;
  diet_profile_t* profile;

  std::string serviceName("DirList");
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(profile, 0), strdup(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<FMS_Data::LsDirOptions_ptr>(&options)).c_str()); 

  if(diet_string_set(diet_parameter(profile, 4), optionsToString, DIET_VOLATILE)){
    msgErrorDiet += "with directory content option values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);

  if (diet_call(profile)) {
    raiseDietMsgException("Error calling DIET service");
  }

  //Output parameters
  if(diet_string_get(diet_parameter(profile, 5), &ls, NULL)){
    msgErrorDiet += " by receiving directory content information";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(profile, 6), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

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


template <class TypeOfOption>
int RemoteFileProxy::transferFile(const std::string& dest, 
                                  const TypeOfOption& options, 
                                  const std::string& serviceName,
                                  FileTransfer& fileTransfer){

  string destHost = FileProxy::extHost(dest);
  string localUser = "";
  bfs::path destPath(FileProxy::extName(dest));
  if(destHost.compare("localhost")==0){
    
  // get the destination full qualified host name 
    
    destHost =vishnu::getLocalMachineName("22");
    
    uid_t uid = getuid();
    struct passwd*  pw = getpwuid(uid);
    localUser = pw->pw_name;

    
    destPath = bfs::current_path().string();
  
    if(dest.compare(".")!=0){
      destPath/= FileProxy::extName(dest);
    }

    if (ba::starts_with(dest,"/")  ){
      destPath=dest;
    }


  }   

  std::string srcHost = getHost();
  std::string srcPath = getPath();    

  char *optionsToString = NULL;
  char *fileTransferInString = NULL;
  diet_profile_t* profile;
  char* errMsg;

  std::string sessionKey=this->getSession().getSessionKey();

  bool isAsyncTransfer = (serviceName.compare("RemoteFileCopyAsync")==0 || serviceName.compare("RemoteFileMoveAsync")==0);
  if(!isAsyncTransfer) {
    profile = diet_profile_alloc(serviceName.c_str(), 6, 6, 7);
  } else {
    profile = diet_profile_alloc(serviceName.c_str(), 6, 6, 8);
  }

  //IN Parameters  

  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
      DIET_VOLATILE);

  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()), DIET_VOLATILE);

  //to set the hostname of the source machine
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(srcHost.c_str()), DIET_VOLATILE);
  //to set the hostname of the destination machine
  diet_string_set(diet_parameter(profile, 3), const_cast<char*>(srcPath.c_str()),DIET_VOLATILE); 

  //to set the hostname of the destination machine
  diet_paramstring_set(diet_parameter(profile, 4), const_cast<char*>(destHost.c_str()), DIET_VOLATILE);
  //to set the destination path of the destination machine
  diet_string_set(diet_parameter(profile, 5), const_cast<char*>(destPath.string().c_str()), DIET_VOLATILE);

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<TypeOfOption*>(&options)).c_str());

  diet_string_set(diet_parameter(profile,6 ), optionsToString, DIET_VOLATILE);

  if(!isAsyncTransfer) { 
    diet_string_set(diet_parameter(profile, 7), NULL, DIET_VOLATILE);
  } else {
    diet_string_set(diet_parameter(profile, 7), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile, 8), NULL, DIET_VOLATILE);
  }

  if (diet_call(profile)) {
    raiseDietMsgException("Error calling DIET service");
  }

  if(!isAsyncTransfer) {
    diet_string_get(diet_parameter(profile, 7), &errMsg, NULL);
    /*To raise a vishnu exception if the received message is not empty*/
    raiseExceptionIfNotEmptyMsg(errMsg);
  } else {
    diet_string_get(diet_parameter(profile, 7), &fileTransferInString, NULL);
    diet_string_get(diet_parameter(profile, 8), &errMsg, NULL);

    /*To raise a vishnu exception if the received message is not empty*/
    raiseExceptionIfNotEmptyMsg(errMsg);
  
    FMS_Data::FileTransfer_ptr fileTransfer_ptr = NULL;

    parseEmfObject(std::string(fileTransferInString), fileTransfer_ptr);

    fileTransfer = *fileTransfer_ptr;

  }
  
return 0;
}

int RemoteFileProxy::cp(const std::string& dest, const CpFileOptions& options) {
  FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, "RemoteFileCopy", fileTransfer);
}

int RemoteFileProxy::mv(const std::string& dest, const CpFileOptions& options) {
  FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, "RemoteFileMove", fileTransfer);
}

int RemoteFileProxy::cpAsync(const std::string& dest, 
                             const CpFileOptions& options,
                             FileTransfer& fileTransfer) {
  return transferFile(dest, options, "RemoteFileCopyAsync", fileTransfer);
}

int RemoteFileProxy::mvAsync(const std::string& dest, 
                             const CpFileOptions& options,
                             FileTransfer& fileTransfer) {
  return transferFile(dest, options, "RemoteFileMoveAsync", fileTransfer);
}
