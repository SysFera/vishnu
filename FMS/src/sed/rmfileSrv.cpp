#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "FileFactory.hpp"

#include "DIET_client.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>

#include "FMSMapper.hpp"

using namespace std;


/* DIET profile construction.
 * Use the serverHostname global variable to create the service name. */
//diet_profile_desc_t* getRemoveFileProfile() {
//  diet_profile_desc_t* result = diet_profile_desc_alloc("FileRemove", 4, 4, 5);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
//
//  return result;
//}

/* mkdir DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveRemoveFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  char* path, *user, *host, *sessionKey, *errMsg = NULL,*optionsSerialized= NULL;

  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_string_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4),&optionsSerialized, NULL);


      localUser = user;
      localPath = path;
      SessionServer sessionServer (sessionKey);
    try {

      //MAPPER CREATION
      Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
      mapperkey = mapper->code("vishnu_remove_file");
      mapper->code(std::string(host)+":"+std::string(path), mapperkey);
      mapper->code(std::string(optionsSerialized), mapperkey);
      cmd = mapper->finalize(mapperkey);

  // check the sessionKey

    sessionServer.check();
   //
    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(host);
    MachineServer machineServer(machine);

    // check the machine
    machineServer.checkMachine();

    // get the machineName
    machineName = machineServer.getMachineName();
    delete machine;

    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(host);

    FileFactory::setSSHServer(machineName);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, acLogin, userKey));

    RmFileOptions_ptr options_ptr= NULL;
 if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_remove_file: RmFileOptions object is not well built");
    }
    file->rm(*options_ptr);
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
    if (errMsg==NULL){
      errMsg = strdup("");
    }

diet_string_set(diet_parameter(profile, 5), errMsg, DIET_VOLATILE);
  return 0;
}
