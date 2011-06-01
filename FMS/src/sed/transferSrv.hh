#ifndef TRANSFERSRV_HH
#define TRANSFERSRV_HH
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/types.h>

#include "FileFactory.hh"

#include "DIET_server.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "ServerFMS.hpp"
#include "FMSMapper.hpp"
#include "ListFileTransfers.hh"

#include "FileTransferServer.hpp"
using namespace std;


/**
 * \brief Function to solve the getListOfJobs service 
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfFileTransfers(diet_profile_t* pb);

diet_profile_desc_t* getTransferFileProfile(const std::string& serviceName);

diet_profile_desc_t* getTransferRemoteFileProfile(const std::string& serviceName);

diet_profile_desc_t* getTransferRemoteFileAsyncProfile(const std::string& serviceName);

diet_profile_desc_t* getFileTransfersListProfile();


  template < File::TransferType transferType> int solveTransferFile(diet_profile_t* profile){

  string localPath, localUser, userKey, head, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *dest, *errMsg = NULL, *optionsSerialized=NULL;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";
 
  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4), &dest, NULL);
  diet_string_get(diet_parameter(profile, 5), &optionsSerialized, NULL);


  std::cout << "Dans solveCopyFile:  " << "\n"; 
  std::cout << "path:  " << path <<"\n"; 
  std::cout << "user:  " << user <<"\n";
  std::cout << "host:  " << host <<"\n"; 
  std::cout << "dest:  " << dest <<"\n"; 


  localUser = user;
  localPath = path;

  std:: string destPath=File::extName(dest);
  std:: string destHost=File::extHost(dest);
  SessionServer sessionServer (sessionKey);

  try {

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    if(transferType==File::copy){
      mapperkey = mapper->code("vishnu_copy_file");
    }
    if(transferType==File::move){
      mapperkey = mapper->code("vishnu_move_file");
    }
    mapper->code(path, mapperkey);
    mapper->code(dest, mapperkey);
    mapper->code(optionsSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // check the sessionKey
    sessionServer.check();
   

    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(destHost);
    MachineServer machineServer(machine);
    
    // check the machine
    machineServer.checkMachine();

    // get the machineName
    machineName = machineServer.getMachineName();
    delete machine;

    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(destHost);

    std::cout << "acLogin: " << acLogin << "\n";
    std::cout << "machineName: " << machineName << "\n";

    FileFactory::setSSHServer(host);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, localUser, userKey));

    CpFileOptions* options_ptr= NULL;
    if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    std::ostringstream destCompletePath;
    destCompletePath << acLogin << "@"<<machineName <<":"<<destPath;
   
    std::cout << "destCompletePath " <<destCompletePath.str() << "\n";
    
   // create a FileTranferServer and register the transfer in database 
    if(transferType==File::copy){
      file->cp(destCompletePath.str(),*options_ptr);
    }

    if (transferType==File::move){
      file->mv(destCompletePath.str(),*options_ptr);
    }

    //To register the command
    sessionServer.finish(cmd, FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    err.appendMsgComp(finishError);    

    errMsg = strdup(err.buildExceptionString().c_str());
  }
  if (errMsg==NULL) {
    errMsg = strdup("");
  }

    diet_string_set(diet_parameter(profile, 6), errMsg, DIET_VOLATILE);
    return 0;


}




template <File::TransferType transferType> int solveTransferRemoteFile(diet_profile_t* profile){

  string  userKey, srcUserLogin,srcMachineName;
  char* srcPath, *destUser, *srcHost,*sessionKey, *destHost,*destPath, *errMsg = NULL, *optionsSerialized=NULL;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &destUser, NULL);
  diet_paramstring_get(diet_parameter(profile, 2), &srcHost, NULL);
  diet_string_get(diet_parameter(profile, 3), &srcPath, NULL);
  diet_paramstring_get(diet_parameter(profile, 4), &destHost, NULL);
  diet_string_get(diet_parameter(profile, 5), &destPath, NULL);
  diet_string_get(diet_parameter(profile, 6), &optionsSerialized, NULL);


  std::cout << "Dans solveCopyFile:  " << "\n"; 
  std::cout << "destUser:  " << destUser <<"\n";
  std::cout << "srcHost:  " << srcHost <<"\n"; 
  std::cout << "srcPath:  " << srcPath <<"\n"; 
  std::cout << "destHost:  " << destHost <<"\n"; 
  std::cout << "destPath:  " << destPath <<"\n"; 

  string destUserLogin(destUser);
  string destMachineName(destHost);
  SessionServer sessionServer (sessionKey);
  
  
     try {

    //MAPPER CREATION
    string destCpltPath = destPath;
    if(std::string(destUser).size()==0){
      destCpltPath = std::string(destHost)+":"+std::string(destPath);
    }
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    if(transferType==File::copy){
      mapperkey = mapper->code("vishnu_copy_file");
    }
    if(transferType==File::move){
      mapperkey = mapper->code("vishnu_move_file");
    }
    mapper->code(std::string(srcHost)+":"+std::string(srcPath), mapperkey);
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
    srcMachineServer.checkMachine();

    // get the source machineName
    srcMachineName = srcMachineServer.getMachineName();
    delete machine;

    // get the source machine user login
    srcUserLogin = UserServer(sessionServer).getUserAccountLogin(srcHost);

    if(strcmp(destUser,"")==0) {

      // get the destination Vishnu machine
      machine = new UMS_Data::Machine();
      machine->setMachineId(destHost);
      MachineServer destMachineServer(machine);

      // check the destination machine
      destMachineServer.checkMachine();

      // get the destination machineName
      destMachineName = destMachineServer.getMachineName();
      delete machine;

      // get the destination  machine user login
      destUserLogin = UserServer(sessionServer).getUserAccountLogin(destHost);

    }

    std::cout << "source user login: " << srcUserLogin << "\n";
    std::cout << "machineName: " << srcMachineName << "\n";

    FileFactory::setSSHServer(srcMachineName);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,srcPath, srcUserLogin, userKey));

    CpFileOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    std::ostringstream destCompletePath;
    destCompletePath << destUserLogin << "@"<<destMachineName <<":"<<destPath;
    std::cout << "destCompletePath " <<destCompletePath.str() << "\n";
   

    if(transferType==File::copy){
      file->cp(destCompletePath.str(),*options_ptr);
    }

    if (transferType==File::move){
      file->mv(destCompletePath.str(),*options_ptr);
    }

    //To register the command
    sessionServer.finish(cmd, FMS, vishnu::CMDSUCCESS);
 
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    err.appendMsgComp(finishError);

    errMsg = strdup(err.buildExceptionString().c_str());
  }
  if (errMsg==NULL) {
    errMsg = strdup("");
  }

  diet_string_set(diet_parameter(profile, 7), errMsg, DIET_VOLATILE);
  return 0;

}

/**
 * \brief Function to solve the generic query service 
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* optionValueSerialized = NULL;
  std::string listSerialized = "";
  std::string empty = "";
  std::string errorInfo;
  int mapperkey;
  std::string cmd;
  std::string finishError ="";

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);

  SessionServer sessionServer  = SessionServer(std::string(sessionKey));

  QueryParameters* options = NULL;
  List* list = NULL;

  try {
    //To parse the object serialized
    if(!parseEmfObject(std::string(optionValueSerialized), options)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }
    QueryType query(options, sessionServer);

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(std::string(optionValueSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //  perform the query
    list = query.list();

    const char* name = "list";
    ::ecorecpp::serializer::serializer _ser(name);
    listSerialized =  _ser.serialize(list);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  delete options;
  delete list;
  return 0;
}

/****************************************************************************************************/

// Asynchronous command area


template <File::TransferType transferType> int solveTransferRemoteFileAsync(diet_profile_t* profile){

  string  userKey, srcUserLogin,srcMachineName;
  char* srcPath, *destUser, *srcHost,*sessionKey, *destHost,*destPath, *errMsg = NULL, *optionsSerialized=NULL;
  std::string finishError ="";
  std::string fileTransferSerialized="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &destUser, NULL);
  diet_paramstring_get(diet_parameter(profile, 2), &srcHost, NULL);
  diet_string_get(diet_parameter(profile, 3), &srcPath, NULL);
  diet_paramstring_get(diet_parameter(profile, 4), &destHost, NULL);
  diet_string_get(diet_parameter(profile, 5), &destPath, NULL);
  diet_string_get(diet_parameter(profile, 6), &optionsSerialized, NULL);

  SessionServer sessionServer (sessionKey);

  try {

    std::cout << "Dans solveCopyFile:  " << "\n"; 
    std::cout << "destUser:  " << destUser <<"\n";
    std::cout << "srcHost:  " << srcHost <<"\n"; 
    std::cout << "srcPath:  " << srcPath <<"\n"; 
    std::cout << "destHost:  " << destHost <<"\n"; 
    std::cout << "destPath:  " << destPath <<"\n"; 

    string destUserLogin(destUser);
    string destMachineName(destHost);
    SessionServer sessionServer (sessionKey);



    //MAPPER CREATION
    string destCpltPath = destPath;
    if(std::string(destUser).size()==0){
      destCpltPath = std::string(destHost)+":"+std::string(destPath);
    }
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    if(transferType==File::copy){
      mapperkey = mapper->code("vishnu_copy_async_file");
    }
    if(transferType==File::move){
      mapperkey = mapper->code("vishnu_move_async_file");
    }
    mapper->code(std::string(srcHost)+":"+std::string(srcPath), mapperkey);
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
    srcMachineServer.checkMachine();

    // get the source machineName
    srcMachineName = srcMachineServer.getMachineName();
    delete machine;

    // get the source machine user login
    srcUserLogin = UserServer(sessionServer).getUserAccountLogin(srcHost);

    if(strcmp(destUser,"")==0) {

      // get the destination Vishnu machine
      machine = new UMS_Data::Machine();
      machine->setMachineId(destHost);
      MachineServer destMachineServer(machine);

      // check the destination machine
      destMachineServer.checkMachine();

      // get the destination machineName
      destMachineName = destMachineServer.getMachineName();
      delete machine;

      // get the destination  machine user login
      destUserLogin = UserServer(sessionServer).getUserAccountLogin(destHost);

    }

    std::cout << "source user login: " << srcUserLogin << "\n";
    std::cout << "machineName: " << srcMachineName << "\n";

    FileFactory::setSSHServer(srcMachineName);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,srcPath, srcUserLogin, userKey));

    CpFileOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    std::ostringstream destCompletePath;
    destCompletePath << destUserLogin << "@"<<destMachineName <<":"<<destPath;
    std::cout << "destCompletePath " <<destCompletePath.str() << "\n";

    int vishnuId=ServerFMS::getInstance()->getVishnuId(); 

    FileTransferServer fileTransferServer(sessionServer, srcHost, destHost, srcPath, destPath,vishnuId);

    if(transferType==File::copy){
      fileTransferServer.addCpThread(*(static_cast<SSHFile*>(file.get())),destCompletePath.str(),*options_ptr);
    }

    if (transferType==File::move){

      fileTransferServer.addMvThread();
    }

    FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();

    FMS_Data::FileTransfer_ptr fileTransfer=ecoreFactory->createFileTransfer();
    
    *fileTransfer= fileTransferServer.getFileTransfer();

    std::cout <<"*********** affichage de fileTransfer ******************\n" ;

    std::cout << "fileTransfer->getStatus()" <<fileTransfer->getStatus() << "\n";
    std::cout << "fileTransfer->getSize()" <<fileTransfer->getSize() << "\n";
    std::cout << "fileTransfer->getTrComand()" <<fileTransfer->getTrCommand() << "\n";
    std::cout << "fileTransfer->getTransferId()" <<fileTransfer->getTransferId() << "\n";
    std::cout << "fileTransfer->getClientMachineId()" <<fileTransfer->getClientMachineId() << "\n";
    std::cout << "fileTransfer->getUserId()" <<fileTransfer->getUserId() << "\n";
    std::cout << "fileTransfer->getSourceMachineId()" <<fileTransfer->getSourceMachineId() << "\n";
    std::cout << "fileTransfer->getDestinationMachineId()" <<fileTransfer->getDestinationMachineId() << "\n";
    std::cout << "fileTransfer->getSourceFilePath()" <<fileTransfer-> getSourceFilePath()<< "\n";
    std::cout << "fileTransfer->getDestinationFilePath()" <<fileTransfer->getDestinationFilePath() << "\n";
    std::cout << "fileTransfer->getStart_time()" <<fileTransfer->getStart_time() << "\n";


    const char* name = "fileTransfer";
    ::ecorecpp::serializer::serializer _ser(name);

    fileTransferSerialized =  _ser.serialize(const_cast<FMS_Data::FileTransfer_ptr>(fileTransfer));

      std::cout << "Coucou  apres serialize \n";
    delete fileTransfer;

    //To register the command
    sessionServer.finish(cmd, FMS, vishnu::CMDSUCCESS);
 
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    err.appendMsgComp(finishError);

    errMsg = strdup(err.buildExceptionString().c_str());
  }
  if (errMsg==NULL) {
    errMsg = strdup("");
  }

  diet_string_set(diet_parameter(profile, 7), strdup(fileTransferSerialized.c_str()),DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 8), errMsg, DIET_VOLATILE);
  return 0;



}



#endif
