#include "internalApiFMS.hpp"
#include <string>
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "ServerXMS.hpp"
#include "FMSMapper.hpp"
#include "MapperRegistry.hpp"

using namespace std;
using namespace FMS_Data;
namespace ba = boost::algorithm;


/* change group Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int solveChangeMode (diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string modeInString = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);
  diet_string_get(profile, 4, modeInString);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  istringstream is (modeInString);
  mode_t mode;
  is >> mode;

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_chmod");
    mapper->code(vishnu::convertToString(mode), mapperkey);
    mapper->code(host + ":" + path, mapperkey);
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

    file->chmod(mode);

    diet_string_set(profile, 1, "");
    diet_string_set(profile, 0, "success");

    //To register the command
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);

  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      err.appendMsgComp(fe.what());
    }
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, err.what());
  }
  return 0;
}




/* change group Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int solveChangeGroup (diet_profile_t* profile) {
  string localPath, localUser,userKey="", acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string group = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);
  diet_string_get(profile, 4, group);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_chgrp");
    mapper->code(group, mapperkey);
    mapper->code(host + ":" + path, mapperkey);
    cmd = mapper->finalize(mapperkey);

    sessionServer.check();

    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(host);
    MachineServer machineServer(machine);

    // check the machine
    machineServer.checkMachine();
    machineName = machineServer.getMachineName();
    delete machine;

    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(host);

    FileFactory ff;
    ff.setSSHServer(machineName);
    boost::scoped_ptr<File> file (ff.getFileServer(sessionServer,localPath, acLogin, userKey));
    file->chgrp(group);

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");

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




/* get Content  Vishnu callback function.
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int contentFile(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_content_of_file");
    mapper->code(host + ":" + path, mapperkey);
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
    boost::scoped_ptr<File> file(ff.getFileServer(sessionServer,localPath, acLogin, userKey));

    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, file->getContent());

    //To register the command
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      err.appendMsgComp(fe.what());
    }
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, err.what());
  }
  return 0;
}


/* get information Vishnu callback function. Proceed to the group change using the
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
  std::string cmd = "";
  std::string fileStatSerialized = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);
  try {
    int mapperkey;
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

    // set success result
    diet_string_set(profile, 1, fileStatSerialized);
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
    if(! vishnu::parseEmfObject(optionsSerialized, options_ptr) ) {
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


/* Global variables defined in the server.cc file. */

/* tail Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n last lines of a file to the client application. */
int solveListDir(diet_profile_t* profile) {
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
    if (!vishnu::parseEmfObject(optionsSerialized, options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_LsDir: LsDirOptions object is not well built");
    }

    ::ecorecpp::serializer::serializer _ser;
    result =  _ser.serialize_str(const_cast<FMS_Data::DirEntryList*>(file->ls(*options_ptr)));

    // set success result
    diet_string_set(profile, 1, result);
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


/* mkdir Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveCreateDir(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
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
    mapperkey = mapper->code("vishnu_create_dir");
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

    FMS_Data::CreateDirOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_create_dir: CreateDirOptions object is not well built");
    }
    file->mkdir(*options_ptr);

    // set success result
    diet_string_set(profile, 1, "");
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


/* mkdir Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveCreateFile(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);

  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_touch");
    mapper->code(host + ":" + path, mapperkey);
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
    file->mkfile();

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");

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


/* mkdir Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveRemoveDir(diet_profile_t* profile) {
  std::string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, user);
  diet_string_get(profile, 3, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);
  try {
    int mapperkey;
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_remove_dir");
    mapper->code(host + ":" + path, mapperkey);
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

    file->rmdir();

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");
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


/* mkdir Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveRemoveFile(diet_profile_t* profile) {

  string localPath, localUser, userKey, acLogin, machineName;
  std::string cmd = "";
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

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  localUser = user;
  localPath = path;
  SessionServer sessionServer (sessionKey);
  try {
    int mapperkey;
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

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");

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


/* tail Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n last lines of a file to the client application. */
int tailFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  std::string path = "";
  std::string user = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
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
    mapperkey = mapper->code("vishnu_tail_of_file");
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

    FMS_Data::TailOfFileOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_Tail: TailOfFileOptions object is not well built");
    }

    // set success result
    diet_string_set(profile, 1, file->tail(*options_ptr));
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


/**
 * \brief Function to solve the getListOfJobs service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfFileTransfers(diet_profile_t* pb) {
  return solveGenerique<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList, ListFileTransfers >(pb);
}



/**
 * \brief Function to solve the File transfer stop service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveFileTransferStop(diet_profile_t* profile) {

  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string cmd = "";

  diet_string_get(profile,0, sessionKey);
  diet_string_get(profile,1, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  FMS_Data::StopTransferOptions_ptr options_ptr = NULL;

  try {
    //MAPPER CREATION
    int mapperkey;
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_stop_file_transfer");
    mapper->code(optionsSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr)) {
      SystemException(ERRCODE_INVDATA, "solve_fileTransferStop: options object is not well built");
    }

    int vishnuId = ServerXMS::getInstance()->getVishnuId();
    boost::shared_ptr<FileTransferServer> fileTransferServer(new FileTransferServer(sessionServer,vishnuId));
    fileTransferServer->stopThread(*options_ptr);

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");
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
