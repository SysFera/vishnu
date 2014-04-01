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
#include "fmsUtils.hpp"
#include "FMSVishnuException.hpp"

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
                                 const string& path)
  : FileProxy(sessionProxy, path), upToDate(false) {}

/* Copy constructor. */
RemoteFileProxy::RemoteFileProxy(const RemoteFileProxy& file)
  : FileProxy(file), upToDate(false) {}

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

  std::string serviceName(SERVICES_FMS[FILEGETINFOS]);
  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);

  //IN Parameters
  diet_string_set(profile, 0, sessionKey);
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

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


  diet_profile_t* chgrpProfile = diet_profile_alloc(serviceName, 4);
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

  if (diet_string_set(chgrpProfile, 2, getHost())){
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(chgrpProfile, 3, group)){
    msgErrorDiet += "with group parameter "+group;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(chgrpProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(chgrpProfile);

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


  chmodProfile = diet_profile_alloc(serviceName, 4);

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

  if(diet_string_set(chmodProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ostringstream os;
  os << mode;
  string modeInString (os.str());

  if (diet_string_set(chmodProfile, 3, modeInString)){
    msgErrorDiet += " by receiving mode parameter "+modeInString;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(chmodProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(chmodProfile);

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


  diet_profile_t* headProfile = diet_profile_alloc(serviceName, 4);

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

  if(diet_string_set(headProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options));


  if(diet_string_set(headProfile, 3, optionsToString)){
    msgErrorDiet += " by receiving head of file options values ";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(headProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(headProfile);

  //Output parameter
  if(diet_string_get(headProfile, 1, fileHead)){
    msgErrorDiet += " by receiving fileHead message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

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
  std::string serviceName(SERVICES_FMS[FILECONTENT]);
  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* getContentProfile = diet_profile_alloc(serviceName,  3);

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

  if(diet_string_set(getContentProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }


  if (diet_call(getContentProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(getContentProfile);

  if(diet_string_get(getContentProfile, 1, fileContent)){
    msgErrorDiet += " by receiving fileContent message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

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


  diet_profile_t* mkfileProfile = diet_profile_alloc(serviceName, 3);

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

  if(diet_string_set(mkfileProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(mkfileProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(mkfileProfile);
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

  diet_profile_t* mkdirProfile = diet_profile_alloc(serviceName, 4);

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

  if(diet_string_set(mkdirProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<CreateDirOptions*>(&options));

  diet_string_set(mkdirProfile, 3, optionsToString);

  if (diet_call(mkdirProfile)){
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(mkdirProfile);

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


  diet_profile_t* rmProfile = diet_profile_alloc(serviceName, 4);

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

  if(diet_string_set(rmProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<RmFileOptions*>(&options));
  diet_string_set(rmProfile, 3, optionsToString);

  if (diet_call(rmProfile)){
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(rmProfile);


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


  diet_profile_t* rmdirProfile = diet_profile_alloc(serviceName, 3);

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

  if(diet_string_set(rmdirProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(rmdirProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(rmdirProfile);

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


  diet_profile_t* tailProfile = diet_profile_alloc(serviceName, 4);

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

  if(diet_string_set(tailProfile, 2, getHost())) {
    msgErrorDiet += "with host parameter "+getHost();
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options));

  if(diet_string_set(tailProfile, 3, optionsToString)){
    msgErrorDiet += " by receiving tail of file options values ";
    raiseCommunicationMsgException(msgErrorDiet);
  }


  if (diet_call(tailProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(tailProfile);

  if(diet_string_get(tailProfile, 1, fileTail)){
    msgErrorDiet += " by receiving fileTail message";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  result = fileTail;
  return result;
}

/* Call the ls Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */

FMS_Data::DirEntryList*
RemoteFileProxy::ls(const LsDirOptions& options) const {

  // serialize the options object
  ::ecorecpp::serializer::serializer _ser;
  string optionsSerialized =  _ser.serialize_str(const_cast<FMS_Data::LsDirOptions_ptr>(&options));

  // initialize profile
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[DIRLIST], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());
  diet_string_set(profile, 3, optionsSerialized);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string rpcResult;
  diet_string_get(profile, 1, rpcResult);
  FMS_Data::DirEntryList* result;
  parseEmfObject(rpcResult, result, "Error by receiving List object serialized");

  return result;
}


int
RemoteFileProxy::transferFile(const std::string& dest,
                              const FMS_Data::CpFileOptions& options,
                              const std::string& serviceName,
                              FileTransfer& fileTransfer) {

  string destHost = FileProxy::extHost(dest);
  bfs::path destPath(FileProxy::extName(dest));

  if (destHost == "localhost") {
    if(dest.compare(".")==0){ // The destination is local:  get its full qualified host name
      destPath=bfs::current_path();
    }
    bfs::system_complete(destPath);
  }

  std::string srcHost = getHost();
  std::string srcPath = getPath();

  std::string sessionKey=this->getSession().getSessionKey();

  diet_profile_t* transferFileProfile = diet_profile_alloc(serviceName, 6);
  diet_string_set(transferFileProfile, 0, sessionKey);
  diet_string_set(transferFileProfile, 1, srcHost);
  diet_string_set(transferFileProfile, 2, srcPath);
  diet_string_set(transferFileProfile, 3, destHost);
  diet_string_set(transferFileProfile, 4, destPath.string());

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::CpFileOptions_ptr>(&options));
  diet_string_set(transferFileProfile,5 , optionsToString);

  if (diet_call(transferFileProfile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(transferFileProfile);

  std::string resultSerialized;
  diet_string_get(transferFileProfile, 1, resultSerialized);
  FMS_Data::FileTransfer_ptr fileTransfer_ptr = NULL;
  parseEmfObject(resultSerialized, fileTransfer_ptr);
  fileTransfer = *fileTransfer_ptr;
  delete fileTransfer_ptr;

  if (vishnu::ifLocalTransferInvolved(fileTransfer.getSourceMachineId(),
                                      fileTransfer.getDestinationMachineId()))
  {
    std::string baseCommand = vishnu::buildTransferBaseCommand(options.getTrCommand(),
                                                               options.isIsRecursive(),
                                                               false,
                                                               0);
    std::string transferCommand = boost::str(boost::format("%1% %2% %3%")
                                             % baseCommand
                                             % fileTransfer.getSourceFilePath()
                                             % fileTransfer.getDestinationFilePath());

    bool isAsyncTransfer = (serviceName == SERVICES_FMS[REMOTEFILECOPYASYNC]
                            || serviceName == SERVICES_FMS[REMOTEFILEMOVEASYNC]);

    std::string errorMsg;
    std::clog << boost::format("preparing transfer... %1%\n") %transferCommand;
    std::cout << "------------ transfer infomation for file " << fileTransfer.getTransferId() << std::endl;
    std::cout << right << "transferId: " << fileTransfer.getTransferId()   << std::endl;
    std::cout << right << "errorMsg: " << fileTransfer.getErrorMsg()  << std::endl;
    std::cout << right << "userId: " << fileTransfer.getUserId()   << std::endl;
    std::cout << right << "clientMachineId: " << fileTransfer.getClientMachineId()   << std::endl;
    std::cout << right << "sourceMachineId: " << fileTransfer.getSourceMachineId()   << std::endl;
    std::cout << right << "destinationMachineId: " << fileTransfer.getDestinationMachineId()   << std::endl;
    std::cout << right << "sourceFilePath: " << fileTransfer.getSourceFilePath()   << std::endl;
    std::cout << right << "destinationFilePath: " << fileTransfer.getDestinationFilePath()   << std::endl;
    std::cout << right << "size: " << fileTransfer.getSize()   << std::endl;

    if (! isAsyncTransfer) {

      vishnu::execSystemCommand(transferCommand, errorMsg);
      std::clog << boost::format("transfer completed.\n");

    } else { // asynchronous
      pid_t pid = fork();
      std::cout << pid <<"\n";
      if (pid < 0) {
        throw FMSVishnuException(ERRCODE_CLI_ERROR_RUNTIME,
                                 "cannot fork process for asynchronous transfer");
      } else if (pid ==0) {

        setsid();  // to avoid death when parent exits

        vishnu::execSystemCommand(transferCommand, errorMsg);
        std::clog << boost::format("transfer completed.\n") ;

      } else {
        /** parent: just completed execution */
      }
    }
    if (! errorMsg.empty()) {
      std::clog << errorMsg <<"\n";
    }
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
