/**
  File: internalApi.cpp
  Creation Date: 04/04/2014
  Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
  */

#include "internalApi.hpp"
#include "SessionServer.hpp"
#include "ListFileTransfers.hpp"
#include <istream>






/**
 * \brief Function to solve the generic query service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return 0 if the service succeeds or an error code otherwise
 */
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb) {

  std::string sessionKey = "";
  std::string optionValueSerialized = "";
  std::string listSerialized = "";
  std::string errorInfo;
  std::string cmd;
  std::string finishError ="";

  //IN Parameters
  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, optionValueSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer  = SessionServer(sessionKey);

  QueryParameters* options = NULL;
  List* list = NULL;

  try {
    int mapperkey;
    //To parse the object serialized
    if (!vishnu::parseEmfObject(optionValueSerialized, options)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }


    QueryType query(sessionKey);

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(optionValueSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    //  perform the query

    list = query.list(options);

    ::ecorecpp::serializer::serializer _ser;

    listSerialized =  _ser.serialize_str(const_cast<List*>(list));

    //OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, listSerialized.c_str());
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    //OUT Parameter
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, errorInfo);
  }
  delete options;
  delete list;
  return 0;
}


/* change group Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int solveChangeGroup (diet_profile_t* profile) {
  std::string localPath, userKey="", acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string group = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, group);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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

    FileFactory fileFactory;
    fileFactory.setSSHServer(machineName);
    boost::scoped_ptr<File> file (fileFactory.getFileServer(sessionServer,localPath, acLogin, userKey));
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



/* change group Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int solveChangeMode (diet_profile_t* profile) {
  std::string localPath;
  std::string userKey;
  std::string acLogin;
  std::string machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string modeInString = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, modeInString);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  std::istringstream is (modeInString);
  mode_t mode;
  is >> mode;

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


/* get Content  Vishnu callback function.
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int contentFile(diet_profile_t* profile) {
  std::string localPath, userKey, acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
  std::string host = "";
  std::string sessionKey = "";
  std::string localPath, userKey, machineName;
  std::string cmd = "";
  std::string fileStatSerialized = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
    boost::scoped_ptr<FMS_Data::FileStat> fileStat_ptr (new FMS_Data::FileStat());

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
  std::string localPath, userKey, acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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


/* tail Vishnu callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n last lines of a file to the client application. */
int solveListDir(diet_profile_t* profile) {
  std::string localPath, userKey, acLogin, machineName;

  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string result = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
  std::string localPath, userKey, acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
  std::string localPath, userKey, acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
  std::string localPath, userKey, acLogin, machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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

  std::string localPath;
  std::string userKey;
  std::string acLogin;
  std::string machineName;
  std::string cmd = "";
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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
  std::string localPath;
  std::string userKey;
  std::string acLogin;
  std::string machineName;
  std::string path = "";
  std::string host = "";
  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string cmd = "";

  diet_string_get(profile, 0, sessionKey);
  diet_string_get(profile, 1, path);
  diet_string_get(profile, 2, host);
  diet_string_get(profile, 3, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

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

    int vishnuId = ServerFMS::getInstance()->getVishnuId();
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



