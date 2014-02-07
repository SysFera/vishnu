#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <pwd.h>

#include "DIET_client.h"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "RemoteFileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "boost/filesystem.hpp"
#include<boost/algorithm/string.hpp>
#include "FMSServices.hpp"

using namespace std;
using namespace FMS_Data;
namespace bfs=boost::filesystem;
namespace ba=boost::algorithm;

using namespace std;
using namespace FMS_Data;

/* Default constructor. */
RemoteFileProxy::RemoteFileProxy() : upToDate(false) {}

/* Standard constructor.
 * Take the file path and user name as parameters.
 */
RemoteFileProxy::RemoteFileProxy(const SessionProxy& sessionProxy,
                                 const string& path,
                                 const string& localUser)
  : FileProxy(sessionProxy, path), upToDate(false), localUser(localUser) {}

/* Copy constructor. */
RemoteFileProxy::RemoteFileProxy(const RemoteFileProxy& file)
  : FileProxy(file), upToDate(false), localUser(file.localUser) {}

/* Standard destructor. */
RemoteFileProxy::~RemoteFileProxy() {
}

/* Returns true if the file informations are up to date. */
bool
RemoteFileProxy::isUpToDate() const {
  return upToDate;
}

/* Get the informations about this remote file. Call the Vishnu service. */
void
RemoteFileProxy::getInfos() const {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILEGETINFOS]);
  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* profile = diet_profile_alloc(serviceName, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(profile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(profile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(profile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(profile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);

  std::string fileStatInString;
  diet_string_get(profile, 1, fileStatInString);

  if (! fileStatInString.empty()) {
    FileStat_ptr fileStat_ptr = NULL;
    parseEmfObject(fileStatInString, fileStat_ptr);
    setFileStat(*fileStat_ptr);
    exists(true);
    upToDate = true;
    delete fileStat_ptr;
  }
}

/* Copy operator. */
RemoteFileProxy&
RemoteFileProxy::operator=(const RemoteFileProxy& file) {
  FileProxy::operator=(file);
  localUser = file.localUser;
  upToDate = file.isUpToDate();
  return *this;
}

/* Call the change group Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::chgrp(const string& group) {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILECHANGEGROUP]);
  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* chgrpProfile = diet_profile_alloc(serviceName, 5);
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(chgrpProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(chgrpProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(chgrpProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(chgrpProfile, 3, getHost())){
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(chgrpProfile, 4, group)){
    msgErrorDiet += "with group parameter "+group;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(chgrpProfile)) {
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if (diet_string_get(chgrpProfile, 0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  return 0;
}

/* Call the change mode Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::chmod(const mode_t mode) {
  diet_profile_t* chmodProfile;
  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILECHANGEMODE]);
  std::string sessionKey=this->getSession().getSessionKey();


  chmodProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //Set parameters
  if(diet_string_set(chmodProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(chmodProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(chmodProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(chmodProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ostringstream os;
  os << mode;
  string modeInString (os.str());

  if (diet_string_set(chmodProfile, 4, modeInString)){
    msgErrorDiet += " by receiving mode parameter "+modeInString;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(chmodProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  //Output parameter
  if (diet_string_get(chmodProfile, 0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  raiseExceptionIfNotEmptyMsg(errMsg);

  return 0;
}

/* Call the file head Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
string
RemoteFileProxy::head(const HeadOfFileOptions& options) {
  std::string result;
  std::string fileHead = "";
  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILEHEAD]);
  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* headProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(headProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(headProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(headProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(headProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options));


  if(diet_string_set(headProfile, 4, optionsToString)){
    msgErrorDiet += " by receiving head of file options values ";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(headProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  //Output parameter
  if(diet_string_get(headProfile, 0, fileHead)){
    msgErrorDiet += " by receiving fileHead message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_get(headProfile, 1, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  raiseExceptionIfNotEmptyMsg(errMsg);

  result = fileHead;
  return result;
}

/* Call the file getContent Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
string
RemoteFileProxy::getContent() {
  std::string fileContent ="";
  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILECONTENT]);
  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* getContentProfile = diet_profile_alloc(serviceName,  4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(getContentProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(getContentProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(getContentProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(getContentProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }


  if (diet_call(getContentProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(getContentProfile, 0, fileContent)){
    msgErrorDiet += " by receiving fileContent message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_get(getContentProfile, 1, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);


  return fileContent;
}

/* Call the mkfile Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::mkfile(const mode_t mode) {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILECREATE]);

  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* mkfileProfile = diet_profile_alloc(serviceName, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(mkfileProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkfileProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkfileProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkfileProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(mkfileProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(mkfileProfile, 0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);

  return 0;
}

/* Call the mkdir Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::mkdir(const CreateDirOptions& options) {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[DIRCREATE]);

  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* mkdirProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(mkdirProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkdirProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkdirProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(mkdirProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<CreateDirOptions*>(&options));

  diet_string_set(mkdirProfile, 4, optionsToString);

  if (diet_call(mkdirProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(mkdirProfile, 0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);

  return 0;
}


/* Call the remove file Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::rm(const RmFileOptions& options) {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[FILEREMOVE]);

  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* rmProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(rmProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<RmFileOptions*>(&options));
  diet_string_set(rmProfile, 4, optionsToString);

  if (diet_call(rmProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(rmProfile,0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);
  return 0;
}

/* Call the remove directory Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::rmdir() {

  std::string errMsg = "";
  std::string serviceName(SERVICES_FMS[DIRREMOVE]);
  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* rmdirProfile = diet_profile_alloc(serviceName, 4);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(rmdirProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmdirProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmdirProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(rmdirProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(rmdirProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(rmdirProfile, 0, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  raiseExceptionIfNotEmptyMsg(errMsg);

  exists(true);
  return 0;


}

/* Call the file tail Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
string
RemoteFileProxy::tail(const TailOfFileOptions& options) {
  string result;
  std::string fileTail = "";
  std::string errMsg = "";


  std::string serviceName(SERVICES_FMS[FILETAIL]);
  std::string sessionKey=this->getSession().getSessionKey();


  diet_profile_t* tailProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(tailProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(tailProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(tailProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(tailProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options));

  if(diet_string_set(tailProfile, 4, optionsToString)){
    msgErrorDiet += " by receiving tail of file options values ";
    raiseCommunicationMsgException(msgErrorDiet);
  }


  if (diet_call(tailProfile)){
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(diet_string_get(tailProfile, 0, fileTail)){
    msgErrorDiet += " by receiving fileTail message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_get(tailProfile, 1, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  raiseExceptionIfNotEmptyMsg(errMsg);

  result = fileTail;
  return result;
}

/* Call the ls Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */

FMS_Data::DirEntryList*
RemoteFileProxy::ls(const LsDirOptions& options) const {
  FMS_Data::DirEntryList* result;
  std::string errMsg = "";
  std::string ls = "";

  std::string serviceName(SERVICES_FMS[DIRLIST]);
  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* lsProfile = diet_profile_alloc(serviceName, 5);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if(diet_string_set(lsProfile, 0, sessionKey)){
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(lsProfile, 1, getPath())){
    msgErrorDiet += "with file path parameter "+getPath();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(lsProfile, 2, localUser)){
    msgErrorDiet += "with local user parameter "+localUser;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_set(lsProfile, 3, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::LsDirOptions_ptr>(&options));

  if(diet_string_set(lsProfile, 4, optionsToString)){
    msgErrorDiet += "with directory content option values ";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(lsProfile)) {
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  //Output parameters
  if(diet_string_get(lsProfile, 0, ls)){
    msgErrorDiet += " by receiving directory content information";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if(diet_string_get(lsProfile, 1, errMsg)){
    msgErrorDiet += " by receiving errorInfo message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  /*To raise a vishnu exception if the received message is not empty*/
  raiseExceptionIfNotEmptyMsg(errMsg);

  parseEmfObject(ls, result, "Error by receiving List object serialized");

  return result;
}


template <class TypeOfOption>
int
RemoteFileProxy::transferFile(const std::string& dest,
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

  std::string fileTransferInString = "";
  std::string errMsg = "";

  std::string sessionKey=this->getSession().getSessionKey();

  bool isAsyncTransfer = (serviceName == SERVICES_FMS[REMOTEFILECOPYASYNC] || serviceName == SERVICES_FMS[REMOTEFILEMOVEASYNC]);

  diet_profile_t* transferFileProfile = diet_profile_alloc(serviceName, 7);
  diet_string_set(transferFileProfile, 0, sessionKey);
  diet_string_set(transferFileProfile, 1, localUser);
  diet_string_set(transferFileProfile, 2, srcHost);
  diet_string_set(transferFileProfile, 3, srcPath);
  diet_string_set(transferFileProfile, 4, destHost);
  diet_string_set(transferFileProfile, 5, destPath.string());

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<TypeOfOption*>(&options));
  diet_string_set(transferFileProfile,6 , optionsToString);

  if (diet_call(transferFileProfile)) {
    raiseCommunicationMsgException("error while contacting the file management service");
  }

  if(!isAsyncTransfer) {
    diet_string_get(transferFileProfile, 0, errMsg);
    raiseExceptionIfNotEmptyMsg(errMsg);
  } else {
    diet_string_get(transferFileProfile, 0, fileTransferInString);
    diet_string_get(transferFileProfile, 1, errMsg);
    raiseExceptionIfNotEmptyMsg(errMsg);

    FMS_Data::FileTransfer_ptr fileTransfer_ptr = NULL;
    parseEmfObject(fileTransferInString, fileTransfer_ptr);
    fileTransfer = *fileTransfer_ptr;

  }

  return 0;
}

int
RemoteFileProxy::cp(const std::string& dest, const CpFileOptions& options) {
  FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILECOPY], fileTransfer);
}

int
RemoteFileProxy::mv(const std::string& dest, const CpFileOptions& options) {
  FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILEMOVE], fileTransfer);
}

int
RemoteFileProxy::cpAsync(const std::string& dest,
                         const CpFileOptions& options,
                         FileTransfer& fileTransfer) {
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILECOPYASYNC], fileTransfer);
}

int
RemoteFileProxy::mvAsync(const std::string& dest,
                         const CpFileOptions& options,
                         FileTransfer& fileTransfer) {
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILEMOVEASYNC], fileTransfer);
}
