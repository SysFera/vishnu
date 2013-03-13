#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "DIET_client.h"
#include "FileFactory.hpp"
#include "File.hpp"
#include "internalApiFMS.hpp"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>
#include "FMSVishnuException.hpp"
#include <boost/algorithm/string.hpp>
#include "FMSMapper.hpp"

using namespace std;
using namespace FMS_Data;
namespace ba = boost::algorithm;

/* get information DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The function returns all the information about a file:
 *  - The local owner
 *  - The local group
 *  - The local uid & gid
 *  - The creation, modification and acces time.
 *  - The file type.
 */
int get_infos(diet_profile_t* profile) {

  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string localPath, localUser, userKey, machineName;
  std::string finishError ="";
  std::string cmd = "";
  std::string fileStatSerialized = "";
  std::string errMsg = "";
  int mapperkey;

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);
  try {

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_stat");
    mapper->code(host + ":" + path, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // check the sessionKey
    sessionServer.check();

    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(host);
    MachineServer machineServer(machine);

    // check the machine
    machineServer.checkMachine();

    // get the machineName
    machineName = machineServer.getMachineName();
    delete machine;


    std::string acLogin = UserServer(sessionServer).getUserAccountLogin(host);

    FileFactory ff;
    ff.setSSHServer(machineName);

    boost::scoped_ptr<File> file (ff.getFileServer(sessionServer,localPath, acLogin, userKey));

    boost::scoped_ptr<FileStat> fileStat_ptr (new FileStat);

    if ( file->exists()) {

      *fileStat_ptr=file->getFileStat();
      ::ecorecpp::serializer::serializer _ser;
      fileStatSerialized = _ser.serialize_str(const_cast<FMS_Data::FileStat_ptr>(fileStat_ptr.get()));

    } else {

      throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, static_cast<SSHFile*>(file.get())->getErrorMsg());
    }

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
    fileStatSerialized="";
  }

  diet_string_set(profile, 4, fileStatSerialized.c_str());
  diet_string_set(profile, 5, errMsg.c_str());
  return 0;
}
