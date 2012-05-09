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


/// DIET profile construction.
//diet_profile_desc_t* getContentProfile() {
//  diet_profile_desc_t* result = diet_profile_desc_alloc("FileContent", 3, 3, 5);
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

/* get Content  DIET callback function.
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int contentFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, content, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *errMsg = NULL, *result = NULL;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";


  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_string_get(diet_parameter(profile, 3), &host, NULL);


  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_content_of_file");
    mapper->code(std::string(host)+":"+std::string(path), mapperkey);
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


      content = file->getContent();
      result = strdup(content.c_str());

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
      result = strdup("");
      errMsg = strdup(err.buildExceptionString().c_str());
    }
    if (errMsg==NULL) {
      errMsg = strdup("");
    }
    else {
     result = strdup("");
    }
    diet_string_set(diet_parameter(profile, 4), result, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile, 5), errMsg, DIET_VOLATILE);
    return 0;
}
