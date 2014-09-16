/**
  File: internalApi.hpp
  Creation Date: 04/04/2014
  Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
  */

#ifndef INTERNALAPI_HPP
#define INTERNALAPI_HPP

#include <boost/make_shared.hpp>

#include "DIET_client.h"
#include "File.hpp"
#include "Mapper.hpp"
#include "MapperRegistry.hpp"
#include "MachineServer.hpp"
#include "FileFactory.hpp"
#include "FileStat.hpp"
#include "ServerXMS.hpp"
#include "FileTransferServer.hpp"

/**
 * \brief the change group solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */

int solveChangeGroup(diet_profile_t* profile);


/**
 * \brief the change mode solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveChangeMode(diet_profile_t* profile);


/**
 * \brief the get file content solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveGetFileContent(diet_profile_t* profile);

/**
 * \brief the get infos solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveGetInfos(diet_profile_t* profile);

/**
 * \brief the head of file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveHeadFile(diet_profile_t* profile);


/**
 * \brief the list directory solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveListDir(diet_profile_t* profile);


/**
 * \brief the mkdir solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveCreateDir(diet_profile_t* profile);

/**
 * \brief the mkfile solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveCreateFile(diet_profile_t* profile);


/**
 * \brief the remove directory solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveRemoveDir(diet_profile_t* profile);

/**
 * \brief the remove file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveRemoveFile(diet_profile_t* profile);

/**
 * \brief the tail of file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveTailFile(diet_profile_t* profile);


/**
 * \brief List file transfer solve function
 * \param profile is a structure which corresponds to the descriptor of a profile
 * \return 0 if the service succeeds
 */
int
solveGetListOfFileTransfers(diet_profile_t* profile);

/**
 * \brief File transfer stop solve function
 * \param profile is a structure which corresponds to the descriptor of a profile
 * \return 0 if the service succeeds or an error code otherwise
 */
int
solveFileTransferStop(diet_profile_t* profile);

/**
 * @brief Update transfer information when the transfer were initiated from the client side
 * @param profile The profile information
 * @return 0 on success, non-zero on erro
 */
int
solveUpdateClientSideTransfer(diet_profile_t* profile);

/**
 * \brief Implementation of file transfer (local to remote) solve function
 * \param profile the service profile
 * \return 0 if the service succeeds or an error code otherwise
 */
template < File::TransferType transferType, File::TransferMode transferMode>
int
solveTransferFile(diet_profile_t* profile){

  std::string srcUserKey = "";
  std::string destUser = "";
  std::string destMachineName = "";
  std::string fileTransferSerialized = "";
  std::string errMsg = "";
  std::string finishError = "";
  std::string srcPath = "";
  std::string srcUser = "";
  std::string srcHost = "";
  std::string sessionKey = "";
  std::string dest = "";
  std::string optionsSerialized = "";

  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, srcPath);
  diet_string_get(profile, 2, srcHost);
  diet_string_get(profile, 3, dest);
  diet_string_get(profile, 4, optionsSerialized);


  // reset profile to handle result
  diet_profile_reset(profile, 2);

  std:: string destPath=File::extName(dest);
  std:: string destHost=File::extHost(dest);
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);

    if (transferMode==File::sync){

      if(transferType==File::copy){
        mapperkey = mapper->code("vishnu_cp");
      }
      if(transferType==File::move){
        mapperkey = mapper->code("vishnu_mv");
      }
    }
    else{

      if(transferType==File::copy){
        mapperkey = mapper->code("vishnu_acp");
      }
      if(transferType==File::move){
        mapperkey = mapper->code("vishnu_amv");
      }

    }

    mapper->code(srcPath, mapperkey);
    mapper->code(dest, mapperkey);
    mapper->code(optionsSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // check the sessionKey
    sessionServer.check();


    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(destHost);
    MachineServer machineServer(machine);

    // check the machine
    if (destHost != "localhost"){
      machineServer.checkMachine();
    }

    // get the machineName
    destMachineName = machineServer.getMachineAddress();
    delete machine;

    // get the acLogin
    if (destHost != "localhost"){
      destUser = UserServer(sessionServer).getUserAccountLogin(destHost);
    } else {
      destUser = destHost;
    }

    // get the acLogin
    if (srcHost != "localhost"){
      srcUser = UserServer(sessionServer).getUserAccountLogin(srcHost);
    } else {
      srcUser = srcHost;
    }



    FMS_Data::CpFileOptions* options_ptr= NULL;
    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    boost::shared_ptr<FileTransferServer> fileTransferServer = \
      boost::make_shared<FileTransferServer>(sessionServer,
                                             srcHost,
                                             destHost,
                                             srcPath,
                                             destPath);

    // Perfor the transfer now
    if (transferMode == File::sync) {
      if (transferType == File::copy) {
        fileTransferServer->addCpThread(srcUser,
                                        srcHost,
                                        srcUserKey,
                                        destUser,
                                        destMachineName,
                                        *options_ptr);
      }

      if (transferType == File::move) {
        fileTransferServer->addMvThread(srcUser,
                                        srcHost,
                                        srcUserKey,
                                        destUser,
                                        destMachineName,
                                        *options_ptr);
      }
    }
    else{

      if(transferType==File::copy){
        fileTransferServer->addCpAsyncThread(srcUser,
                                             srcHost,
                                             srcUserKey,
                                             destUser,
                                             destMachineName,
                                             *options_ptr);
      }

      if (transferType==File::move){

        fileTransferServer->addMvAsyncThread(srcUser,
                                             srcHost,
                                             srcUserKey,
                                             destUser,
                                             destMachineName,
                                             *options_ptr);
      }


    }
    FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();

    FMS_Data::FileTransfer_ptr fileTransfer=ecoreFactory->createFileTransfer();

    *fileTransfer= fileTransferServer->getFileTransfer();
    delete fileTransfer;


    ::ecorecpp::serializer::serializer _ser;

    fileTransferSerialized =  _ser.serialize_str(const_cast<FMS_Data::FileTransfer_ptr>(fileTransfer));
    //To register the command
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);

  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    err.appendMsgComp(finishError);

    errMsg = err.buildExceptionString();
  }

  if (errMsg.empty()){
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, fileTransferSerialized.c_str());
  } else {
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, errMsg);
  }

  return 0;
}


/**
 * \brief Implementation of file transfer (from remote to local or remote ) solve function
 * \param profile the service profile
 * \return 0 if the service succeeds or an error code otherwise
 */

template <File::TransferType transferType, File::TransferMode transferMode>
int
solveTransferRemoteFile(diet_profile_t* profile){

  std::string srcPath = "";
  std::string destUser = "";
  std::string srcHost = "";
  std::string sessionKey = "";
  std::string destHost = "";
  std::string destPath = "";
  std::string optionsSerialized = "";
  std::string srcUserKey = "";
  std::string srcUserLogin = "";
  std::string srcMachineName = "";
  std::string errMsg = "";
  std::string finishError = "";
  std::string fileTransferSerialized = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, srcHost);
  diet_string_get(profile, 2, srcPath);
  diet_string_get(profile, 3, destHost);
  diet_string_get(profile, 4, destPath);
  diet_string_get(profile, 5, optionsSerialized);

  // reset profile to handle result
  diet_profile_reset(profile, 2);

  SessionServer sessionServer (sessionKey);

  try {

    int mapperkey;
    std::string destUserLogin(destUser);
    std::string destMachineName(destHost);
    SessionServer sessionServer (sessionKey);

    // check the source machine
    // and get the source machine if applied
    if (destHost != "localhost"){
      UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
      machine->setMachineId(destHost);
      MachineServer destMachineServer(machine);
      destMachineServer.checkMachine();
      destMachineName = destMachineServer.getMachineAddress();
      delete machine;
    } else {
      destMachineName = destHost;
    }
    // get the source machine user login
    if (destHost != "localhost"){
      destUserLogin = UserServer(sessionServer).getUserAccountLogin(destHost);
    } else {
      destUserLogin = destUser;
    }

    //MAPPER CREATION
    std::string destCpltPath = destPath;
    if (destUser.empty()) {
      destCpltPath = destHost + ":" + destPath;
    }

    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    if (transferMode == File::sync) {
      if(transferType == File::copy) {
        mapperkey = mapper->code("vishnu_cp");
      }
      if(transferType == File::move) {
        mapperkey = mapper->code("vishnu_mv");
      }
    } else{
      if (transferType == File::copy) {
        mapperkey = mapper->code("vishnu_acp");
      }
      if (transferType == File::move) {
        mapperkey = mapper->code("vishnu_amv");
      }

    }
    mapper->code(srcHost + ":" + srcPath, mapperkey);
    mapper->code(destCpltPath, mapperkey);
    mapper->code(optionsSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // check the sessionKey
    sessionServer.check();

    // get the source Vishnu machine
    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(srcHost);
    MachineServer srcMachineServer(machine);

    // check the source machine
    if (srcHost != "localhost"){
      srcMachineServer.checkMachine();
      srcMachineName = srcMachineServer.getMachineAddress();
    } else {
      srcMachineName = srcHost;
    }

    delete machine;

    // get the source machine user login
    if (srcHost != "localhost"){
      srcUserLogin = UserServer(sessionServer).getUserAccountLogin(srcHost);
    } else {
      srcUserLogin = destUser;
    }

    if (destUser.empty()) {
      // get the destination Vishnu machine
      machine = new UMS_Data::Machine();
      machine->setMachineId(destHost);

      // check the destination machine
      if (destHost != "localhost"){
        MachineServer destMachineServer(machine);
        destMachineServer.checkMachine();
        // get the destination machineName
        destMachineName = destMachineServer.getMachineAddress();
        delete machine;
      }

      // get the destination  machine user login
      if (destHost != "localhost"){
        destUserLogin = UserServer(sessionServer).getUserAccountLogin(destHost);
      } else {
        destUserLogin = destUser;
      }
    }

    FMS_Data::CpFileOptions_ptr options_ptr= NULL;
    if (! vishnu::parseEmfObject(optionsSerialized, options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    boost::shared_ptr<FileTransferServer> fileTransferServer = \
      boost::make_shared<FileTransferServer>(sessionServer,
                                             srcHost,
                                             destHost,
                                             srcPath,
                                             destPath);
    // Perfor the transfer now
    if (transferMode==File::sync) {

      if (transferType == File::copy) {
        fileTransferServer->addCpThread(srcUserLogin,
                                        srcMachineName,
                                        srcUserKey,
                                        destUserLogin,
                                        destMachineName,
                                        *options_ptr);
      }
      if (transferType == File::move) {
        fileTransferServer->addMvThread(srcUserLogin,
                                        srcMachineName,
                                        srcUserKey,
                                        destUserLogin,
                                        destMachineName,
                                        *options_ptr);
      }

    } else {
      if (transferType == File::copy) {
        fileTransferServer->addCpAsyncThread(srcUserLogin,
                                             srcMachineName,
                                             srcUserKey,
                                             destUserLogin,
                                             destMachineName,
                                             *options_ptr);
      }
      if (transferType == File::move) {
        fileTransferServer->addMvAsyncThread(srcUserLogin,
                                             srcMachineName,
                                             srcUserKey,
                                             destUserLogin,
                                             destMachineName,
                                             *options_ptr);
      }
    }
    FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();

    boost::scoped_ptr<FMS_Data::FileTransfer> fileTransfer(ecoreFactory->createFileTransfer());

    *(fileTransfer.get()) = fileTransferServer->getFileTransfer();

    ::ecorecpp::serializer::serializer _ser;

    fileTransferSerialized = _ser.serialize_str(const_cast<FMS_Data::FileTransfer_ptr>(fileTransfer.get()));

    //To register the command
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);

  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    err.appendMsgComp(finishError);

    errMsg = err.buildExceptionString().c_str();
  }

  if (errMsg.empty()){
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, fileTransferSerialized.c_str());
  } else {
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, errMsg);
  }
  return 0;
}

#endif // INTERNALAPI_HPP
