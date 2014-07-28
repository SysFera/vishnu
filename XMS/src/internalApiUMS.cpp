/**
* \file UMS/src/sed/internalApi.cpp
* \brief This file implements the internal api of UMS
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "internalApiUMS.hpp"
#include <string>
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "ServerXMS.hpp"
#include "vishnu_version.hpp"
#include "VersionManager.hpp"
#include "ObjectIdServer.hpp"
#include "ExportServer.hpp"
#include "ExportFactory.hpp"

using namespace vishnu;

/**
* \brief Function to solve the service sessionConnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionConnect(diet_profile_t* pb) {
  std::string userId;
  std::string password;
  std::string clientKey;
  std::string clientHostname;
  std::string options;
  std::string version;

  // IN Parameters
  diet_string_get(pb, 0, userId);
  diet_string_get(pb, 1, password);
  diet_string_get(pb, 2, clientKey);
  diet_string_get(pb, 3, clientHostname);
  diet_string_get(pb, 4, options);
  diet_string_get(pb, 5, version);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  UserServer userServer = UserServer(userId, password);
  MachineClientServer machineClientServer =  MachineClientServer(clientKey, clientHostname);
  SessionServer sessionServer;

  UMS_Data::ConnectOptions_ptr connectOpt = NULL;
  UMS_Data::Version_ptr versionClient = NULL;
  UMS_Data::Version_ptr versionServer = NULL;

  try {
    // To parse the object serialized
    if (!parseEmfObject(version, versionClient)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    versionServer = vishnu::parseVersion(VISHNU_VERSION);
    if (versionServer == NULL) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The format of the VISHNU VERSION of the server is incorrect");
    }
    VersionManager versionManager(versionClient, versionServer);
    if (!versionManager.isCompatible()) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, versionManager.getError());
    }
    // To parse the object serialized
    if (!parseEmfObject(options, connectOpt)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    sessionServer.connectSession(userServer, machineClientServer, connectOpt);

    // To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    UMS_Data::Session session = sessionServer.getData();
    std::string sessionSerializedUpdate = _ser.serialize_str(const_cast<UMS_Data::Session_ptr>(&session));

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, sessionSerializedUpdate);
  } catch (VishnuException& e) {
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, e.what());
  }

  delete connectOpt;
  delete versionClient;
  delete versionServer;

  return 0;
}

/**
* \brief Function to solve the service sessionReconnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionReconnect(diet_profile_t* pb) {
  std::string userId;
  std::string password;
  std::string clientKey;
  std::string clientHostname;
  std::string sessionId;
  std::string version;

  // IN Parameters
  diet_string_get(pb, 0, userId);
  diet_string_get(pb, 1, password);
  diet_string_get(pb, 2, clientKey);
  diet_string_get(pb, 3, clientHostname);
  diet_string_get(pb, 4, sessionId);
  diet_string_get(pb, 5, version);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  UserServer userServer = UserServer(userId, password);
  MachineClientServer machineClientServer =  MachineClientServer(clientKey, clientHostname);


  SessionServer sessionServer;
  sessionServer.getData().setSessionId(sessionId);
  UMS_Data::Version_ptr versionClient = NULL;
  UMS_Data::Version_ptr versionServer = NULL;

  try {
    // To parse the object serialized
    if (!parseEmfObject(version, versionClient)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }
    versionServer = vishnu::parseVersion(VISHNU_VERSION);
    if (versionServer == NULL) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The parameter version of the server is mal formed");
    }
    VersionManager versionManager(versionClient, versionServer);
    if (!versionManager.isCompatible()) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, versionManager.getError());
    }

    sessionServer.reconnect(userServer, machineClientServer, sessionId);
    // To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    UMS_Data::Session session = sessionServer.getData();
    std::string sessionSerializedUpdate = _ser.serialize_str(const_cast<UMS_Data::Session_ptr>(&session));

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, sessionSerializedUpdate);
  } catch (VishnuException& ex) {
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1,  ex.what());
  }

  delete versionClient;
  delete versionServer;

  return 0;
}

/**
* \brief Function to solve the service sessionClose
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionClose(diet_profile_t* pb) {
  std::string sessionKey;
  std::string cmd;

  // IN Parameter
  diet_string_get(pb, 0, sessionKey);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_close");
    cmd = mapper->finalize(mapperkey);

    sessionServer.close();

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1);

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS, "", false);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }

    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  return 0;
}

/**
* \brief Function to solve the service userCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserCreate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, userSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User_ptr user = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_user");
    mapper->code(userSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(userSerialized, user)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    userServer.init();
    userServer.add(user,ServerXMS::getInstance()->getSendmailScriptPath());

    // To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    std::string userSerializedUpdate = _ser.serialize_str(user);

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, userSerializedUpdate);
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS, user->getUserId());
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }

  delete user;

  return 0;
}

/**
* \brief Function to solve the service solveUserUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserUpdate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, userSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User_ptr user = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_user");
    mapper->code(userSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (! parseEmfObject(userSerialized, user)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    };

    userServer.init();
    userServer.update(user);

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());

  }
  delete user;
  return 0;
}

/**
* \brief Function to solve the service solveUserDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserDelete(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, userId);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User user;
  user.setUserId(userId);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_user");
    mapper->code(userId, mapperkey);
    cmd = mapper->finalize(mapperkey);

    userServer.init();
    userServer.deleteUser(user);

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  return 0;
}

/**
* \brief Function to solve the service solveUserPasswordChange
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordChange(diet_profile_t* pb) {
  std::string userId;
  std::string password;
  std::string newPassword;
  std::string version;

  // IN Parameters
  diet_string_get(pb, 0, userId);
  diet_string_get(pb, 1, password);
  diet_string_get(pb, 2, newPassword);
  diet_string_get(pb, 3, version);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  UserServer userServer = UserServer(userId, password);
  UMS_Data::Version_ptr versionClient = NULL;
  UMS_Data::Version_ptr versionServer = NULL;

  try {
    // To parse the object serialized
    if (!parseEmfObject(version, versionClient)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }
    versionServer = vishnu::parseVersion(VISHNU_VERSION);
    if (versionServer == NULL) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The format of the VISHNU VERSION of the server is incorrect");
    }
    VersionManager versionManager(versionClient, versionServer);
    if (!versionManager.isCompatible()) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, versionManager.getError());
    }
    userServer.changePassword(newPassword);

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
  } catch (VishnuException& ex) {
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  return 0;
}

/**
* \brief Function to solve the service solveUserPasswordReset
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordReset(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, userId);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User user;
  user.setUserId(userId);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_reset_password");
    mapper->code(userId, mapperkey);
    cmd = mapper->finalize(mapperkey);

    userServer.init();
    userServer.resetPassword(user, ServerXMS::getInstance()->getSendmailScriptPath());

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, user.getPassword());

    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS, "", false);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED, "", false);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  return 0;
}

/**
* \brief Function to solve the service solveMachineCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineCreate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string machineSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, machineSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::Machine_ptr machine = NULL;

  try {
    int mapperkey;
    std::string msgComp = "The ssh public key file content is invalid";

    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_machine");
    mapper->code(machineSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(machineSerialized, machine, msgComp)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
    }

    MachineServer machineServer = MachineServer(machine, sessionServer);
    machineServer.add();

    // To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    std::string machineSerializedUpdate = _ser.serialize_str(machine);

    // set success result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, machineSerializedUpdate);

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS, machine->getMachineId());
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete machine;
  return 0;
}

/**
* \brief Function to solve the service solveMachineUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineUpdate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string machineSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, machineSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::Machine_ptr machine = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_machine");
    mapper->code(machineSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(machineSerialized, machine)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    MachineServer machineServer = MachineServer(machine, sessionServer);
    machineServer.update();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameter
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete machine;
  return 0;
}

/**
* \brief Function to solve the service solveMachineDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineDelete(diet_profile_t* pb) {
  std::string sessionKey;
  std::string machineId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, machineId);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  UMS_Data::Machine* machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);

  SessionServer sessionServer = SessionServer(sessionKey);
  MachineServer machineServer = MachineServer(machine, sessionServer);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_machine");
    mapper->code(machineId, mapperkey);
    cmd = mapper->finalize(mapperkey);

    machineServer.deleteMachine();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameter
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete machine;
  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountCreate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string laccountSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, laccountSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::LocalAccount_ptr localAccount = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_local_account");
    mapper->code(laccountSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(laccountSerialized, localAccount)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    LocalAccountServer localAccountServer (localAccount, sessionServer);
    localAccountServer.add();

    // OUT Parameters
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1);
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }

    // OUT Parameters
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete localAccount;
  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountUpdate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string laccountSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, laccountSerialized);


  // reset profile to handle result
  diet_profile_reset(pb, 2);


  SessionServer sessionServer = SessionServer(sessionKey);
  UMS_Data::LocalAccount_ptr localAccount = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_local_account");
    mapper->code(laccountSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(laccountSerialized, localAccount)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    LocalAccountServer localAccountServer (localAccount, sessionServer);
    localAccountServer.update();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }

    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete localAccount;
  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountDelete(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userId;
  std::string machineId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, userId);
  diet_string_get(pb, 2, machineId);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UMS_Data::LocalAccount *localAccount = new UMS_Data::LocalAccount();
  localAccount->setUserId(userId);
  localAccount->setMachineId(machineId);

  LocalAccountServer localAccountServer (localAccount, sessionServer);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_local_account");
    mapper->code(userId, mapperkey);
    mapper->code(machineId, mapperkey);
    cmd = mapper->finalize(mapperkey);

    localAccountServer.deleteLocalAccount();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }

  delete localAccount;
  return 0;
}

/**
* \brief Function to solve the service solveGenerique
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb) {
  std::string sessionKey;
  std::string optionValueSerialized;
  std::string listSerialized = "";
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, optionValueSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer  = SessionServer(sessionKey);

  QueryParameters* options = NULL;
  List* list = NULL;

  try {
    int mapperkey;
    // To parse the object serialized
    if (!parseEmfObject(optionValueSerialized, options)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    QueryType query(sessionServer);

    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(optionValueSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    list = query.list(options);

    ::ecorecpp::serializer::serializer _ser;
    listSerialized =  _ser.serialize_str(list);

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, listSerialized);
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete options;
  delete list;
  return 0;
}

/**
* \brief Function to solve the service solveListUsers
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListUsers(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListUsersOptions, UMS_Data::ListUsers, ListUsersServer>(pb);
}

/**
* \brief Function to solve the service solveListMachines
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListMachines(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListMachineOptions, UMS_Data::ListMachines, ListMachinesServer>(pb);
}

/**
* \brief Function to solve the service solveListLocalAccount
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts, ListLocalAccountsServer>(pb);
}

/**
* \brief Function to solve the service solveListOptions
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues, ListOptionsValuesServer>(pb);
}

/**
* \brief Function to solve the service solveListHistoryCmd
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListCmdOptions, UMS_Data::ListCommands, ListCommandsServer>(pb);
}

/**
* \brief Function to solve the service solveListLocalAccount
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListSessionOptions, UMS_Data::ListSessions, ListSessionsServer>(pb);
}


/**
* \brief Function to solve the service solveRestore
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveRestore(diet_profile_t* pb) {

  std::string sqlcode;
  diet_string_get(pb, 0, sqlcode);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  DbFactory factory;
  try {
    Database* db = factory.getDatabaseInstance();
    db->process(sqlcode);

    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
  } catch (VishnuException& ex) {
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthCreate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string authSystemSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, authSystemSerialized);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::AuthSystem_ptr authSystem = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_auth_system");
    mapper->code(authSystemSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(authSystemSerialized, authSystem)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    AuthSystemServer authSystemServer(authSystem, sessionServer);
    authSystemServer.add();

    // To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    std::string authSystemSerializedUpdate = _ser.serialize_str(authSystem);

    // OUT Parameters
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, authSystemSerializedUpdate);

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete authSystem;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthUpdate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string authSystemSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, authSystemSerialized);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UMS_Data::AuthSystem_ptr authSystem = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_auth_system");
    mapper->code(authSystemSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(authSystemSerialized, authSystem)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    AuthSystemServer authSystemServer(authSystem, sessionServer);
    authSystemServer.update();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameter
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete authSystem;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthDelete(diet_profile_t* pb) {
  std::string sessionKey;
  std::string authSystemId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, authSystemId);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  UMS_Data::AuthSystem_ptr authSystem = new UMS_Data::AuthSystem();
  authSystem->setAuthSystemId(authSystemId);

  SessionServer sessionServer = SessionServer(sessionKey);
  AuthSystemServer authSystemServer(authSystem, sessionServer);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_auth_system");
    mapper->code(authSystemId, mapperkey);
    cmd = mapper->finalize(mapperkey);
    authSystemServer.deleteAuthSystem();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameter
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete authSystem;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthList
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthList(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListAuthSysOptions, UMS_Data::ListAuthSystems, ListAuthSystemsServer>(pb);
}

/**
* \brief Function to solve the service solveAccountAuthCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthCreate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string accountSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, accountSerialized);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::AuthAccount_ptr authAccount = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_auth_account");
    mapper->code(accountSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(accountSerialized, authAccount)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    AuthAccountServer authAccountServer(authAccount, sessionServer);
    authAccountServer.add();

    // OUT Parameters
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");
    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameters
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete authAccount;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthUpdate(diet_profile_t* pb) {
  std::string sessionKey;
  std::string accountSerialized;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, accountSerialized);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  UMS_Data::AuthAccount_ptr authAccount = NULL;

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_auth_account");
    mapper->code(accountSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    // To parse the object serialized
    if (!parseEmfObject(accountSerialized, authAccount)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }

    AuthAccountServer authAccountServer(authAccount, sessionServer);
    authAccountServer.update();

    // OUT Parameters
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameters
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }
  delete authAccount;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthDelete(diet_profile_t* pb) {
  std::string sessionKey;
  std::string userId;
  std::string authSystemId;
  std::string cmd;

  // IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, authSystemId);
  diet_string_get(pb, 2, userId);

  // reset the profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  UMS_Data::AuthAccount *authAccount = new UMS_Data::AuthAccount();
  authAccount->setUserId(userId);
  authAccount->setAuthSystemId(authSystemId);

  AuthAccountServer authAccountServer = AuthAccountServer(authAccount, sessionServer);

  try {
    int mapperkey;
    // MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_auth_account");
    mapper->code(userId, mapperkey);
    mapper->code(authSystemId, mapperkey);
    cmd = mapper->finalize(mapperkey);

    authAccountServer.deleteAuthAccount();

    // OUT Parameter
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, "");

    // To save the connection
    sessionServer.finish(cmd, UMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, UMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    // OUT Parameter
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, ex.what());
  }

  delete authAccount;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthList
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthList(diet_profile_t* pb) {
  return solveGenerique<UMS_Data::ListAuthAccOptions, UMS_Data::ListAuthAccounts, ListAuthAccountsServer>(pb);
}


int
solveExport(diet_profile_t* pb){

  std::string sessionKey;
  std::string oldSessionId;
  std::string filename ;
  std::string content;
  int mapperkey;
  std::string cmd;
  std::string error;
  std::string retErr = "";

  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, oldSessionId);
  diet_string_get(pb,2, filename);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_export_commands");
    mapper->code(std::string(oldSessionId), mapperkey);
    mapper->code(std::string(filename), mapperkey);
    cmd = mapper->finalize(mapperkey);
    sessionServer.check();

    // Creating the process server with the options
    ExportServer* exp = ExportFactory::getExporter(userServer);

    // Exporting and setting the results in content
    exp->exporte(oldSessionId, content);

    // Setting out diet param
    diet_string_set(pb,3, content.c_str());
    diet_string_set(pb,4, retErr.c_str());

    // Finishing the command as a success
    sessionServer.finish(cmd, UMS, CMDSUCCESS);
  } catch (VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, UMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(pb,3, "");
    diet_string_set(pb,4, retErr.c_str());
  }
  return 0;
}
