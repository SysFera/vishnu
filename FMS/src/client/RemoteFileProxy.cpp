#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <pwd.h>

//#include "DIET_data.h"
#include "DIET_client.h"
//#include "DIET_Dagda.h"
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

  diet_profile_t* getInfosProfile;

  char *fileStatInString=NULL, * errMsg=NULL;

  std::string serviceName("FileGetInfos");

  std::string sessionKey=this->getSession().getSessionKey();


  getInfosProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(getInfosProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getInfosProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){  
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getInfosProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getInfosProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(getInfosProfile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(getInfosProfile, 5), NULL, DIET_VOLATILE);

  if (diet_call(getInfosProfile)) {
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(getInfosProfile, 4), &fileStatInString, NULL)){
    msgErrorDiet += " by receiving FileStat serialized  content";
    raiseDietMsgException(msgErrorDiet);
  }


  if(diet_string_get(diet_parameter(getInfosProfile, 5), &errMsg, NULL)){
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
  diet_profile_t* chgrpProfile;
  char* errMsg;

  std::string serviceName("FileChangeGroup");

  std::string sessionKey=this->getSession().getSessionKey();


  chgrpProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(chgrpProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chgrpProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chgrpProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet); 
  }

  if(diet_string_set(diet_parameter(chgrpProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chgrpProfile, 4), const_cast<char*>(group.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with group parameter "+group;
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(chgrpProfile, 5), NULL, DIET_VOLATILE);

  if (diet_call(chgrpProfile)) {
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(chgrpProfile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  return 0;
}

/* Call the change mode DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
int RemoteFileProxy::chmod(const mode_t mode) {
  diet_profile_t* chmodProfile;
  char* errMsg;

  std::string serviceName("FileChangeMode");

  std::string sessionKey=this->getSession().getSessionKey();


  chmodProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(chmodProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chmodProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chmodProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(chmodProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ostringstream os;
  os << mode;
  string modeInString (os.str());

  if(diet_string_set(diet_parameter(chmodProfile, 4), const_cast<char*>(modeInString.c_str()),
      DIET_VOLATILE)){
    msgErrorDiet += " by receiving mode parameter "+modeInString;
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(chmodProfile, 5), NULL, DIET_VOLATILE);


  if (diet_call(chmodProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  //Output parameter
  if(diet_string_get(diet_parameter(chmodProfile, 5), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);
  
  return 0;
}

/* Call the file head DIET server.
 * If something goes wrong, throw a raiseDietMsgException containing
 * the error message.
 */
string RemoteFileProxy::head(const HeadOfFileOptions& options) {
  string result;
  char* fileHead, * errMsg;
  diet_profile_t* headProfile;

  std::string serviceName("FileHead");

  std::string sessionKey=this->getSession().getSessionKey();


  headProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters  
  if(diet_string_set(diet_parameter(headProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(headProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(headProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(headProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options));


  if(diet_string_set(diet_parameter(headProfile, 4), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE)){
    msgErrorDiet += " by receiving head of file options values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(headProfile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(headProfile, 6), NULL, DIET_VOLATILE);

  if (diet_call(headProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }
  
  //Output parameter
  if(diet_string_get(diet_parameter(headProfile, 5), &fileHead, NULL)){
    msgErrorDiet += " by receiving fileHead message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(headProfile, 6), &errMsg, NULL)){
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
  
  char* fileContent, * errMsg;
  diet_profile_t* getContentProfile;

  std::string serviceName("FileContent");

  std::string sessionKey=this->getSession().getSessionKey();

  getContentProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(getContentProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getContentProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getContentProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(getContentProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(getContentProfile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(getContentProfile, 5), NULL, DIET_VOLATILE);

  if (diet_call(getContentProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(getContentProfile, 4), &fileContent, NULL)){
    msgErrorDiet += " by receiving fileContent message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(getContentProfile, 5), &errMsg, NULL)){
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
  diet_profile_t* mkfileProfile;
  char* errMsg;
  std::string serviceName("FileCreate");

  std::string sessionKey=this->getSession().getSessionKey();


  mkfileProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(mkfileProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkfileProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkfileProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkfileProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(mkfileProfile, 4), NULL, DIET_VOLATILE);

  if (diet_call(mkfileProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(mkfileProfile, 4), &errMsg, NULL)){
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
int RemoteFileProxy::mkdir(const CreateDirOptions& options) {
  diet_profile_t* mkdirProfile;
  char* errMsg;
  std::string serviceName("DirCreate");

  std::string sessionKey=this->getSession().getSessionKey();


  //mkdirProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  mkdirProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(diet_parameter(mkdirProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkdirProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkdirProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(mkdirProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<CreateDirOptions*>(&options));
  
  diet_string_set(diet_parameter(mkdirProfile,4 ), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(mkdirProfile, 5), NULL, DIET_VOLATILE);

  if (diet_call(mkdirProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(mkdirProfile, 5), &errMsg, NULL)){
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
  diet_profile_t* rmProfile;
  char* errMsg;
  std::string serviceName("FileRemove");

  std::string sessionKey=this->getSession().getSessionKey();


  rmProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(rmProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<RmFileOptions*>(&options)).c_str();
  
  diet_string_set(diet_parameter(rmProfile,4 ), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE);
  
  diet_string_set(diet_parameter(rmProfile,5), NULL, DIET_VOLATILE);

  if (diet_call(rmProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(rmProfile,5), &errMsg, NULL)){
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
  diet_profile_t* rmdirProfile;
  char* errMsg;

  std::string serviceName("DirRemove");

  std::string sessionKey=this->getSession().getSessionKey();


  rmdirProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters    
  if(diet_string_set(diet_parameter(rmdirProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmdirProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmdirProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(rmdirProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(rmdirProfile, 4), NULL, DIET_VOLATILE);

  if (diet_call(rmdirProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(rmdirProfile, 4), &errMsg, NULL)){
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
  char* fileTail, * errMsg;
  diet_profile_t* tailProfile;

  std::string serviceName("FileTail");
  std::string sessionKey=this->getSession().getSessionKey();


  tailProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(tailProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(tailProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(tailProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(tailProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options));

  if(diet_string_set(diet_parameter(tailProfile, 4), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE)){
    msgErrorDiet += " by receiving tail of file options values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(tailProfile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(tailProfile, 6), NULL, DIET_VOLATILE);

  if (diet_call(tailProfile)){
    raiseDietMsgException("error while contacting the file management service");
  }

  if(diet_string_get(diet_parameter(tailProfile, 5), &fileTail, NULL)){
     msgErrorDiet += " by receiving fileTail message";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(tailProfile, 6), &errMsg, NULL)){
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

FMS_Data::DirEntryList* RemoteFileProxy::ls(const LsDirOptions& options) const {

  FMS_Data::DirEntryList* result;
  char* errMsg, *ls;
  diet_profile_t* lsProfile;

  std::string serviceName("DirList");
  std::string sessionKey=this->getSession().getSessionKey();


  lsProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters 
  if(diet_string_set(diet_parameter(lsProfile, 0), const_cast<char*>(sessionKey.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(lsProfile, 1), const_cast<char*>(getPath().c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(lsProfile, 2), const_cast<char*>(localUser.c_str()),
        DIET_VOLATILE)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_set(diet_parameter(lsProfile, 3), const_cast<char*>(getHost().c_str()),
        DIET_VOLATILE)) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::LsDirOptions_ptr>(&options)).c_str();

  if(diet_string_set(diet_parameter(lsProfile, 4), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE)){
    msgErrorDiet += "with directory content option values ";
    raiseDietMsgException(msgErrorDiet);
  }

  diet_string_set(diet_parameter(lsProfile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(lsProfile, 6), NULL, DIET_VOLATILE);

  if (diet_call(lsProfile)) {
    raiseDietMsgException("error while contacting the file management service");
  }

  //Output parameters
  if(diet_string_get(diet_parameter(lsProfile, 5), &ls, NULL)){
    msgErrorDiet += " by receiving directory content information";
    raiseDietMsgException(msgErrorDiet);
  }

  if(diet_string_get(diet_parameter(lsProfile, 6), &errMsg, NULL)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseDietMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  parseEmfObject(std::string(ls), result, "Error by receiving List object serialized");

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

  if (destHost.compare("localhost") == 0 || !vishnu::isNotIP(destHost)) {

	  // The destination is local:  get its full qualified host name
    if (vishnu::isNotIP(destHost)){
	  destHost =vishnu::getLocalMachineName("22");
    }

	  uid_t uid = getuid();
	  struct passwd*  pw = getpwuid(uid);
	  localUser = pw->pw_name;

	  if(dest.compare(".")==0){
		  destPath=bfs::current_path();
	  }

// build a complete local path
	  bfs::system_complete(destPath);

  }

  std::string srcHost = getHost();
  std::string srcPath = getPath();

  char *fileTransferInString = NULL;
  diet_profile_t* transferFileProfile;
  char* errMsg;

  std::string sessionKey=this->getSession().getSessionKey();

  bool isAsyncTransfer = (serviceName.compare("RemoteFileCopyAsync")==0 || serviceName.compare("RemoteFileMoveAsync")==0);
  if(!isAsyncTransfer) {
    transferFileProfile = diet_profile_alloc(serviceName.c_str(), 6, 6, 7);
  } else {
    transferFileProfile = diet_profile_alloc(serviceName.c_str(), 6, 6, 8);
  }

  //IN Parameters

  diet_string_set(diet_parameter(transferFileProfile, 0), const_cast<char*>(sessionKey.c_str()),
      DIET_VOLATILE);

  diet_string_set(diet_parameter(transferFileProfile, 1), const_cast<char*>(localUser.c_str()), DIET_VOLATILE);

  //to set the hostname of the source machine
  diet_string_set(diet_parameter(transferFileProfile, 2), const_cast<char*>(srcHost.c_str()), DIET_VOLATILE);
  //to set the hostname of the destination machine
  diet_string_set(diet_parameter(transferFileProfile, 3), const_cast<char*>(srcPath.c_str()),DIET_VOLATILE);

  //to set the hostname of the destination machine
  diet_string_set(diet_parameter(transferFileProfile, 4), const_cast<char*>(destHost.c_str()), DIET_VOLATILE);
  //to set the destination path of the destination machine
  diet_string_set(diet_parameter(transferFileProfile, 5), const_cast<char*>(destPath.string().c_str()), DIET_VOLATILE);

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<TypeOfOption*>(&options)).c_str();

  diet_string_set(diet_parameter(transferFileProfile,6 ), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE);

  if(!isAsyncTransfer) {
    diet_string_set(diet_parameter(transferFileProfile, 7), NULL, DIET_VOLATILE);
  } else {
    diet_string_set(diet_parameter(transferFileProfile, 7), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(transferFileProfile, 8), NULL, DIET_VOLATILE);
  }

  if (diet_call(transferFileProfile)) {
    raiseDietMsgException("error while contacting the file management service");
  }

  if(!isAsyncTransfer) {
    diet_string_get(diet_parameter(transferFileProfile, 7), &errMsg, NULL);
    /*To raise a vishnu exception if the received message is not empty*/
    raiseExceptionIfNotEmptyMsg(errMsg);
  } else {
    diet_string_get(diet_parameter(transferFileProfile, 7), &fileTransferInString, NULL);
    diet_string_get(diet_parameter(transferFileProfile, 8), &errMsg, NULL);

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
