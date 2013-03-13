#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "FileFactory.hpp"

#include "DIET_client.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"

#include "FMSMapper.hpp"
#include "MapperRegistry.hpp"

using namespace std;

/* Global variables defined in the server.cc file. */

/* tail DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n last lines of a file to the client application. */
int solveListDir(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  FMS_Data::DirEntryList* ls;

  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string finishError ="";
  std::string result = "";
  std::string errMsg = "" ;
  int mapperkey;
  std::string cmd = "";


  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);
  diet_string_get(profile, 4, optionsSerialized);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

    try {

      //MAPPER CREATION
      Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
      mapperkey = mapper->code("vishnu_ls");
      mapper->code(host + ":" + path, mapperkey);
      mapper->code(optionsSerialized, mapperkey);
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


    FileFactory ff;
    ff.setSSHServer(machineName);

    boost::scoped_ptr<File> file (ff.getFileServer(sessionServer,localPath, acLogin, userKey));

    FMS_Data::LsDirOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_LsDir: LsDirOptions object is not well built");
    }

    ls = file->ls(*options_ptr);

    ::ecorecpp::serializer::serializer _ser;

    result =  _ser.serialize_str(const_cast<FMS_Data::DirEntryList*>(ls));

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
      result = "";
    }
    diet_string_set(profile, 5, result.c_str());
    diet_string_set(profile, 6, errMsg.c_str());
    return 0;
}
