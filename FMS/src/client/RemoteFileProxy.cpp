#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include "DIET_client.h"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "RemoteFileProxy.hpp"
#include "boost/filesystem.hpp"
#include<boost/algorithm/string.hpp>
#include "FMSServices.hpp"
#include "fmsUtils.hpp"
#include "FMSVishnuException.hpp"
#include "constants.hpp"

namespace bfs=boost::filesystem;
namespace ba=boost::algorithm;

/* Default constructor. */
RemoteFileProxy::RemoteFileProxy() : upToDate(false) {}

/* Standard constructor.
 * Take the file path and user name as parameters.
 */
RemoteFileProxy::RemoteFileProxy(const SessionProxy& sessionProxy,
                                 const std::string& path)
  : FileProxy(sessionProxy, path), upToDate(false) {}

/* Copy constructor. */
RemoteFileProxy::RemoteFileProxy(const RemoteFileProxy& file)
  : FileProxy(file), upToDate(false) {}

/* Standard destructor. */
RemoteFileProxy::~RemoteFileProxy() {
}

/* Returns true if the file informations are up to date. */
bool
RemoteFileProxy::isUpToDate() const
{
  return upToDate;
}

/* Get the informations about this remote file. Call the Vishnu service. */
void
RemoteFileProxy::getInfos() const
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILEGETINFOS], 3);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);

  std::string fileStatInString;
  diet_string_get(profile, 1, fileStatInString);

  if (! fileStatInString.empty()) {
    FMS_Data::FileStat_ptr fileStat_ptr = NULL;
    parseEmfObject(fileStatInString, fileStat_ptr);
    setFileStat(*fileStat_ptr);
    exists(true);
    upToDate = true;
    delete fileStat_ptr;
  }
  diet_profile_free(profile);
}

/* Copy operator. */
RemoteFileProxy&
RemoteFileProxy::operator=(const RemoteFileProxy& file)
{
  FileProxy::operator=(file);
  upToDate = file.isUpToDate();
  return *this;
}

/* Call the change group Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::chgrp(const std::string& group)
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILECHANGEGROUP], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());
  diet_string_set(profile, 3, group);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
  return 0;
}

/* Call the change mode Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::chmod(const mode_t mode)
{
  //Set parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILECHANGEMODE], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());
  diet_string_set(profile, 3, vishnu::convertToString(mode));

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/* Call the file head Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
std::string
RemoteFileProxy::head(const FMS_Data::HeadOfFileOptions& options)
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILEHEAD], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  ::ecorecpp::serializer::serializer _ser;
  diet_string_set(profile, 3,
                  _ser.serialize_str(const_cast<FMS_Data::HeadOfFileOptions_ptr>(&options)));

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  //Output parameter
  std::string fileHead = "";
  diet_string_get(profile, 1, fileHead);
  diet_profile_free(profile);
  return fileHead;
}

/* Call the file getContent Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
std::string
RemoteFileProxy::getContent() {

  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILECONTENT],  3);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string fileContent ="";
  diet_string_get(profile, 1, fileContent);

  diet_profile_free(profile);
  return fileContent;
}

/* Call the mkfile Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::mkfile(const mode_t mode) {

  std::string serviceName(SERVICES_FMS[FILECREATE]);

  std::string sessionKey=this->getSession().getSessionKey();


  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  diet_string_set(profile, 0, sessionKey);
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);
  exists(true);

  diet_profile_free(profile);
  return 0;
}

/* Call the mkdir Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::mkdir(const FMS_Data::CreateDirOptions& options)
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[DIRCREATE], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  ::ecorecpp::serializer::serializer _ser;
  std::string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::CreateDirOptions*>(&options));

  diet_string_set(profile, 3, optionsToString);

  if (diet_call(profile)){
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  exists(true);

  diet_profile_free(profile);
  return 0;
}


/* Call the remove file Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::rm(const FMS_Data::RmFileOptions& options)
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILEREMOVE],  4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  ::ecorecpp::serializer::serializer _ser;
  std::string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::RmFileOptions*>(&options));
  diet_string_set(profile, 3, optionsToString);

  if (diet_call(profile)){
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);


  exists(true);

  diet_profile_free(profile);
  return 0;
}

/* Call the remove directory Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
int
RemoteFileProxy::rmdir()
{
  //IN Parameters
  diet_profile_t*  profile = diet_profile_alloc(SERVICES_FMS[DIRREMOVE], 3);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  if (diet_call( profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult( profile);

  exists(true);

  diet_profile_free(profile);
  return 0;
}

/* Call the file tail Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */
std::string
RemoteFileProxy::tail(const FMS_Data::TailOfFileOptions& options)
{
  //IN Parameters
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[FILETAIL], 4);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, getPath());
  diet_string_set(profile, 2, getHost());

  ::ecorecpp::serializer::serializer _ser;
  std::string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::TailOfFileOptions_ptr>(&options));
  diet_string_set(profile, 3, optionsToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string fileTail = "";
  diet_string_get(profile, 1, fileTail);

  diet_profile_free(profile);

  return fileTail;
}

/* Call the ls Vishnu server.
 * If something goes wrong, throw a raiseCommunicationMsgException containing
 * the error message.
 */

FMS_Data::DirEntryList*
RemoteFileProxy::ls(const FMS_Data::LsDirOptions& options) const
{
  // serialize the options object
  ::ecorecpp::serializer::serializer _ser;
  std::string optionsSerialized =  _ser.serialize_str(const_cast<FMS_Data::LsDirOptions_ptr>(&options));

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

  diet_profile_free(profile);
  return result;
}

/**
   * \brief A generic class to handle a local to remote file transfer
   * \param dest the destination
   * \param options the transfer options
   * \param serviceName the name of the service
   * \param fileTransfer information about the transfer
   * \param tell whether the transfer should be done asynchroneous or not
   * \return 0 if the function succeeds or an error code otherwise
   */
int
RemoteFileProxy::transferFile(const std::string& dest,
                              const FMS_Data::CpFileOptions& options,
                              const std::string& serviceName,
                              FMS_Data::FileTransfer& fileTransfer,
                              int async)
{
  std::string destHost = FileProxy::extHost(dest);
  bfs::path destPath(FileProxy::extName(dest));

  if (destHost == "localhost") {
    if(dest.compare(".")==0){ // The destination is local:  get its full qualified host name
      destPath=bfs::current_path();
    }
    bfs::system_complete(destPath);
  }

  std::string srcHost = getHost();
  std::string srcPath = getPath();

  diet_profile_t* profile = diet_profile_alloc(serviceName, 6);
  diet_string_set(profile, 0, this->getSession().getSessionKey());
  diet_string_set(profile, 1, srcHost);
  diet_string_set(profile, 2, srcPath);
  diet_string_set(profile, 3, destHost);
  diet_string_set(profile, 4, destPath.string());

  ::ecorecpp::serializer::serializer _ser;
  std::string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::CpFileOptions_ptr>(&options));
  diet_string_set(profile,5 , optionsToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string resultSerialized;
  diet_string_get(profile, 1, resultSerialized);
  FMS_Data::FileTransfer_ptr fileTransfer_ptr = NULL;
  parseEmfObject(resultSerialized, fileTransfer_ptr);
  fileTransfer = *fileTransfer_ptr;
  delete fileTransfer_ptr;

  int direction;
  if (!vishnu::ifLocalTransferInvolved(fileTransfer.getSourceMachineId(), 
									  fileTransfer.getDestinationMachineId(), 
									  direction))
  {
    diet_profile_free(profile);
    return 0;
  }
  std::string baseCommand = vishnu::buildTransferCommand(options.getTrCommand(),
                                                           options.isIsRecursive(),
                                                           false,
                                                           0);
  std::string transferCommand = boost::str(boost::format("%1% %2% %3% 2>&1")
                                             % baseCommand
                                             % fileTransfer.getSourceFilePath()
                                             % fileTransfer.getDestinationFilePath());

    bool isAsyncTransfer = (serviceName == SERVICES_FMS[REMOTEFILECOPYASYNC]
                            || serviceName == SERVICES_FMS[REMOTEFILEMOVEASYNC]);

    std::string errorMsg;
    if (isAsyncTransfer) {
      pid_t pid = fork();
      if (pid < 0) {
        transfer.setErrorMsg("cannot fork process for asynchronous transfer");
        finalizeTransfer(transfer, direction);
        throw FMSVishnuException(ERRCODE_CLI_ERROR_RUNTIME, transfer.getErrorMsg());
      } else if (pid ==0) {
        setsid();  // detach the session
		try{
          vishnu::execSystemCommand(transferCommand, errorMsg);
          fileTransfer.setErrorMsg(errorMsg);
          finalizeTransfer(fileTransfer, direction);
          exit(0);
        } catch (const VishnuException& ex) {
        std::cerr << ex.what()<<"\n";
        exit (1);
        }
      } else {
        int status;
        waitpid(pid, &status, 0);
        if (! WIFEXITED(status) || WEXITSTATUS(status) != 0) {
          std::cerr << "[ERROR] Transfer thread terminated unexpectedly\n";
        }
      }
    } else { // synchronous transfer
      vishnu::execSystemCommand(transferCommand, errorMsg);
      fileTransfer.setErrorMsg(errorMsg);
      finalizeTransfer(transfer, direction);
    }

    if (! errorMsg.empty()) {
      std::clog << errorMsg <<"\n";
    }
  }

  diet_profile_free(profile);
  return 0;
}

int
RemoteFileProxy::cp(const std::string& dest, const FMS_Data::CpFileOptions& options)
{
  FMS_Data::FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILECOPY], fileTransfer, vishnu::TRANFER_SYNC);
}

int
RemoteFileProxy::mv(const std::string& dest, const FMS_Data::CpFileOptions& options)
{
  FMS_Data::FileTransfer fileTransfer; //empty fileTransfer info, the cp function not fills this object structure
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILEMOVE], fileTransfer, vishnu::TRANFER_SYNC);
}

int
RemoteFileProxy::cpAsync(const std::string& dest,
                         const FMS_Data::CpFileOptions& options,
                         FMS_Data::FileTransfer& fileTransfer)
{
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILECOPYASYNC], fileTransfer, vishnu::TRANFER_ASYNC);
}

int
RemoteFileProxy::mvAsync(const std::string& dest,
                         const FMS_Data::CpFileOptions& options,
                         FMS_Data::FileTransfer& fileTransfer)
{
  return transferFile(dest, options, SERVICES_FMS[REMOTEFILEMOVEASYNC], fileTransfer, vishnu::TRANFER_ASYNC);
}


/**
 * @brief Request the server to update the state of transfer performed locally
 * @param transfer The transfer information
 * @param direction The direction of the transfer (local->remote, remote->local)
 * @return throw execption on error
 */
void
RemoteFileProxy::finalizeTransfer(FMS_Data::FileTransfer& transfer, int direction)
{
  //Set parameters

  std::cout << "transfer ID: "<< transfer.getTransferId()<<" \n";
  if (! transfer.getErrorMsg().empty()) {
    transfer.setStatus(vishnu::TRANSFER_FAILED);
  } else {
    transfer.setStatus(vishnu::TRANSFER_COMPLETED);
  }
  diet_profile_t* profile = diet_profile_alloc(SERVICES_FMS[UPDATECLIENTSIDETRANSFER], 2);
  diet_string_set(profile, 0, this->getSession().getSessionKey());

  try {
    if (direction == vishnu::CopyLocalRemote) {
      if (! boost::filesystem::is_directory(transfer.getSourceFilePath())) {
        transfer.setSize(boost::filesystem::file_size(transfer.getSourceFilePath()));
      } else {
        std::string path = (boost::format("%1%/%2%")
                            % transfer.getSourceFilePath()
                            % boost::filesystem::path(transfer.getDestinationFilePath()).filename().string()).str();
        transfer.setSize(boost::filesystem::file_size(path));
      }
    } else {
      if (! boost::filesystem::is_directory(transfer.getDestinationFilePath())) {
        transfer.setSize(boost::filesystem::file_size(transfer.getDestinationFilePath()));
      } else {
        std::string path = (boost::format("%1%/%2%")
                            % transfer.getDestinationFilePath()
                            % boost::filesystem::path(transfer.getSourceFilePath()).filename().string()).str();
        transfer.setSize(boost::filesystem::file_size(path));
      }
    }
  } catch (const boost::filesystem::filesystem_error ex) {
    throw FMSVishnuException(ERRCODE_SYSTEM, ex.what());
  }

  ::ecorecpp::serializer::serializer _ser;
  diet_string_set(profile, 1, _ser.serialize_str(const_cast<FMS_Data::FileTransfer*>(&transfer)));

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
}
