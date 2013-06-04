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




/* mkdir Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveRemoveFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  std::string finishError ="";
  std::string cmd = "";
  std::string errMsg = "";
  int mapperkey;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";

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
	  mapperkey = mapper->code("vishnu_remove_file");
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

	  FMS_Data::RmFileOptions_ptr options_ptr= NULL;
	  if(!vishnu::parseEmfObject(optionsSerialized, options_ptr )) {
		throw SystemException(ERRCODE_INVDATA, "solve_remove_file: RmFileOptions object is not well built");
	  }
	  file->rm(*options_ptr);
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

  diet_string_set(profile, 5, errMsg.c_str());
  return 0;
}
