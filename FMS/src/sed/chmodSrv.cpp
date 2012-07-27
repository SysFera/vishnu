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


///// DIET profile construction.
//diet_profile_desc_t* getChangeModeProfile() {
//  diet_profile_desc_t* result = diet_profile_desc_alloc("FileChangeMode", 4, 4, 5);
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

/* change group DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int solveChangeMode (diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *modeInString;
  std::string finishError ="";
  std::string cmd = "";
  std::string errMsg = "";
  int mapperkey;

  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_string_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4), &modeInString, NULL);

  istringstream is (modeInString);
  mode_t mode;
  is >> mode;

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {

   //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_chmod");
    mapper->code(vishnu::convertToString(mode), mapperkey);
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

    file->chmod(mode);

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
    errMsg = err.buildExceptionString();
  }

  diet_string_set(diet_parameter(profile, 5), const_cast<char*>(errMsg.c_str()), DIET_VOLATILE);
  return 0;
}
