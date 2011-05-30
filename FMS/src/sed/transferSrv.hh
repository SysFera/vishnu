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

#include "FMSMapper.hpp"

using namespace std;


diet_profile_desc_t* getTransferFileProfile(const std::string& serviceName);

diet_profile_desc_t* getTransferRemoteFileProfile(const std::string& serviceName);



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



#endif
