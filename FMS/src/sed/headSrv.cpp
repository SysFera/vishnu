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
#include "MapperRegistry.hpp"

using namespace std;



/* head Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int headFile(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string result = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);
  diet_string_get(profile, 4, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_head_of_file");
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

    FMS_Data::HeadOfFileOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Head: HeadOfFileOptions object is not well built");
    }

    // set success result
    diet_string_set(profile, 1, file->head(*options_ptr));
    diet_string_set(profile, 0, "success");

    //To register the command
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      err.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, err.what());
  }
  return 0;
}
