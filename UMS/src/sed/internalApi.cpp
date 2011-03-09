/**
* \file internalApi.cpp
* \brief This file implements the internal api of UMS
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "internalApi.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"

using namespace vishnu;

/**
* \brief Function to solve the service sessionConnect
* \fn    void solveSessionConnect(diet_profile_t* pb);
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionConnect(diet_profile_t* pb) {

  char *userId = NULL;
  char *password = NULL;
  char *clientKey = NULL;
  char *clientHostname = NULL;
  char *options = NULL;
  std::string empty("");
  std::string errorInfo;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  diet_string_get(diet_parameter(pb,4), &options, NULL);

  UserServer userServer = UserServer(std::string(userId), std::string(password));
  MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
  SessionServer sessionServer("");

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;
  ConnectOptions_ptr connectOpt = parser.load(std::string(options))->as< ConnectOptions >();

  try {
    sessionServer.connectSession(userServer, machineClientServer, connectOpt);
    //To serialize the user object
    const char* name = "solveConnect";
    ::ecorecpp::serializer::serializer _ser(name);
    UMS_Data::Session session = sessionServer.getData();
    std::string sessionSerializedUpdate = _ser.serialize(const_cast<UMS_Data::Session_ptr>(&session));

    //OUT Parameters
    diet_string_set(diet_parameter(pb,5), strdup(sessionSerializedUpdate.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameters
      diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}
/**
* \brief Function to solve the service sessionReconnect
* \fn    void solveSessionReconnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionReconnect(diet_profile_t* pb) {

  char *userId = NULL;
  char *password = NULL;
  char *clientKey = NULL;
  char *clientHostname = NULL;
  char *sessionId = NULL;
  std::string empty("");
  std::string errorInfo;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  diet_string_get(diet_parameter(pb,4), &sessionId, NULL);

  UserServer userServer = UserServer(std::string(userId), std::string(password));
  MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));


  SessionServer sessionServer = SessionServer(std::string(""));

  sessionServer.getData().setSessionId(std::string(sessionId));

  try {
      sessionServer.reconnect(userServer, machineClientServer, std::string(sessionId));
      //To serialize the user object
      const char* name = "solveReconnect";
      ::ecorecpp::serializer::serializer _ser(name);
      UMS_Data::Session session = sessionServer.getData();
      std::string sessionSerializedUpdate = _ser.serialize(const_cast<UMS_Data::Session_ptr>(&session));

      //OUT Parameters
      diet_string_set(diet_parameter(pb,5), strdup(sessionSerializedUpdate.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}
/**
* \brief Function to solve the service sessionClose
* \fn    int solveSessionClose(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionClose(diet_profile_t* pb) {

  char *sessionKey = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameter
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  try {
    sessionServer.close();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_close");
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,1), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT parameter
      diet_string_set(diet_parameter(pb,1), strdup(errorInfo.c_str()), DIET_VOLATILE);
}
  return 0;
}

/**
* \brief Function to solve the service userCreate
* \fn    int solveUserCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserCreate(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *userSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &userSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;
  User_ptr user = parser.load(std::string(userSerialized))->as< User >();

  try {
    userServer.init();
    userServer.add(user);

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_user");
    mapper->code(std::string(userSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //To serialize the user object
    const char* name = "solveUserCreate";
    ::ecorecpp::serializer::serializer _ser(name);
    std::string userSerializedUpdate = _ser.serialize(user);
    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(userSerializedUpdate.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

/**
* \brief Function to solve the service solveUserUpdate
* \fn    int solveUserUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserUpdate(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *userSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &userSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;

  User_ptr user = parser.load(std::string(userSerialized))->as< User >();

  try {
    userServer.init();
    userServer.update(user);

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_user");
    mapper->code(std::string(userSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}
/**
* \brief Function to solve the service solveUserDelete
* \fn    int solveUserDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserDelete(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *userId = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &userId, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User user;
  user.setUserId(userId);

  try {
    userServer.init();
    userServer.deleteUser(user);

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_user");
    mapper->code(std::string(userId), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
    //OUT parameter
      errorInfo =  e.buildExceptionString();
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}
/**
* \brief Function to solve the service solveUserPasswordChange
* \fn    int solveUserPasswordChange(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordChange(diet_profile_t* pb) {

  char *userId = NULL;
  char *password = NULL;
  char *newPassword = NULL;
  std::string empty("");
  std::string errorInfo;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  diet_string_get(diet_parameter(pb,2), &newPassword, NULL);

  UserServer userServer = UserServer(std::string(userId), std::string(password));

  try {
    userServer.init();
    userServer.changePassword(std::string(newPassword));
    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

/**
* \brief Function to solve the service solveUserPasswordReset
* \fn    int solveUserPasswordReset(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordReset(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *userId = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &userId, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  UMS_Data::User user;
  user.setUserId(userId);

  try {
    userServer.init();
    userServer.resetPassword(user);

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_reset_password");
    mapper->code(std::string(userId), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup((user.getPassword()).c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

/**
* \brief Function to solve the service solveMachineCreate
* \fn    int solveMachineCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineCreate(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *machineSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try {

    Machine_ptr machine;
    std::string msgComp = "invalid machine content (ex: sshPublicKeyFile contains special charchar (< or > or double quote)";
    if(parseEmfObject(std::string(machineSerialized), machine, msgComp)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp); 
    };
    
    MachineServer machineServer = MachineServer(machine, sessionServer);
    machineServer.add();

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_machine");
    mapper->code(std::string(machineSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //To serialize the user object
    const char* name = "solveMachineCreate";
    ::ecorecpp::serializer::serializer _ser(name);
    std::string machineSerializedUpdate = _ser.serialize(machine);
    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(machineSerializedUpdate.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
    errorInfo =  e.buildExceptionString();
    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  } 

  return 0;
}
/**
* \brief Function to solve the service solveMachineUpdate
* \fn    int solveMachineUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineUpdate(diet_profile_t* pb) {

  char *sessionKey = NULL;
  char *machineSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineSerialized, NULL);

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  //Parse the model
  ecorecpp::parser::parser parser;
  Machine_ptr machine = parser.load(std::string(machineSerialized))->as< Machine >();
  MachineServer machineServer = MachineServer(machine, sessionServer);

  try {
    machineServer.update();
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_machine");
    mapper->code(std::string(machineSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}
/**
* \brief Function to solve the service solveMachineDelete
* \fn    int solveMachineDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineDelete(diet_profile_t* pb) {

  char *sessionKey = NULL;
  char *machineId = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);

  UMS_Data::Machine* machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  MachineServer machineServer = MachineServer(machine, sessionServer);

  try {
    machineServer.deleteMachine();
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_machine");
    mapper->code(std::string(machineId), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  delete machine;
  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountCreate
* \fn    int solveLocalAccountCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountCreate(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *laccountSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &laccountSerialized, NULL);

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  //Parse the model
  ecorecpp::parser::parser parser;
  LocalAccount_ptr localAccount = parser.load(std::string(laccountSerialized))->as< LocalAccount >();
  LocalAccountServer localAccountServer = LocalAccountServer(localAccount, sessionServer);

  try {
    localAccountServer.add();

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_local_account");
    mapper->code(std::string(laccountSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);


    //OUT Parameters
    diet_string_set(diet_parameter(pb,2), strdup(localAccountServer.getPublicKey().c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();

      //OUT Parameters
      diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountUpdate
* \fn    int solveLocalAccountUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountUpdate(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *laccountSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &laccountSerialized, NULL);

  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  //Parse the model
  ecorecpp::parser::parser parser;
  LocalAccount_ptr localAccount = parser.load(std::string(laccountSerialized))->as< LocalAccount >();
  LocalAccountServer localAccountServer = LocalAccountServer(localAccount, sessionServer);

  try {
    localAccountServer.update();

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_update_local_account");
    mapper->code(std::string(laccountSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
* \brief Function to solve the service solveLocalAccountDelete
* \fn    int solveLocalAccountDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountDelete(diet_profile_t* pb) {

  char *sessionKey = NULL;
  char *userId = NULL;
  char *machineId = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &userId, NULL);
  diet_string_get(diet_parameter(pb,2), &machineId, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UMS_Data::LocalAccount *localAccount = new UMS_Data::LocalAccount();
  localAccount->setUserId(userId);
  localAccount->setMachineId(machineId);

  LocalAccountServer localAccountServer = LocalAccountServer(localAccount, sessionServer);

  try {
    localAccountServer.deleteLocalAccount();
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_delete_local_account");
    mapper->code(std::string(userId), mapperkey);
    mapper->code(std::string(machineId), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  delete localAccount;
  return 0;
}


/**
* \brief Function to solve the service solveConfigurationSave
* \fn    int solveConfigurationSave(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveConfigurationSave(diet_profile_t* pb) {

  char *sessionKey = NULL;
  std::string empty("");
  std::string configurationSerialized("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameter
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  ConfigurationServer configurationServer = ConfigurationServer(sessionServer);

  try {
    configurationServer.save();
    const char* name = "ConfigurationSave";
    ::ecorecpp::serializer::serializer _ser(name);
    configurationSerialized =  _ser.serialize(configurationServer.getData());

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_save_configuration");
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameters
    diet_string_set(diet_parameter(pb,1), strdup(configurationSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();

      //OUT Parameters
      diet_string_set(diet_parameter(pb,1), strdup(configurationSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
* \brief Function to solve the service solveConfigurationRestore
* \fn    int solveConfigurationRestore(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveConfigurationRestore(diet_profile_t* pb) {

  char *sessionKey = NULL;
  char *configurationSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &configurationSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try {

    std::string msgComp = "invalid file content is not a valid restauration content";
    Configuration_ptr configuration;
    if(parseEmfObject(std::string(configurationSerialized), configuration, msgComp)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
    };


    ConfigurationServer configurationServer = ConfigurationServer(configuration, sessionServer);

    configurationServer.restore();
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_restore_configuration");
    mapper->code(std::string(configurationSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (VishnuException& e) {
    errorInfo =  e.buildExceptionString();
    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  } 

  return 0;
}


/**
* \brief Function to solve the service solveOptionValueSet
* \fn    int solveOptionValueSet(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveOptionValueSet(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *optionValueSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);

  // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  // Parse the model
  ecorecpp::parser::parser parser;
  UMS_Data::OptionValue_ptr optionValue = parser.load(optionValueSerialized)->as< UMS_Data::OptionValue >();

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  OptionValueServer optionValueServer = OptionValueServer(optionValue, sessionServer);

  try {
    optionValueServer.configureOption();
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_configure_option");
    mapper->code(std::string(optionValueSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

/**
* \brief Function to solve the service solveOptionValueSetDefault
* \fn    int solveOptionValueSetDefault(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveOptionValueSetDefault(diet_profile_t* pb) {
  char *sessionKey = NULL;
  char *optionValueSerialized = NULL;
  std::string empty("");
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);

  //CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;
  UMS_Data::OptionValue_ptr optionValue = parser.load(optionValueSerialized)->as< UMS_Data::OptionValue >();

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  OptionValueServer optionValueServer = OptionValueServer(optionValue, sessionServer);

  try {
    optionValueServer.configureOption(true);
    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_configure_default_option");
    mapper->code(std::string(optionValueSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
* \brief Function to solve the service solveGenerique
* \fn int solveGenerique(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* optionValueSerialized = NULL;
  std::string listSerialized = "";
  std::string empty = "";
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;



  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);

  // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  // Parse the model
  ecorecpp::parser::parser parser;
  QueryParameters* options = parser.load(optionValueSerialized)->as< QueryParameters >();

  SessionServer sessionServer  = SessionServer(std::string(sessionKey));
  QueryType query(options, sessionServer);

  try {

    List* list = query.list();

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(std::string(optionValueSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    const char* name = "list";
    ::ecorecpp::serializer::serializer _ser(name);
    listSerialized =  _ser.serialize(list);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();

      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

return 0;

}

/**
* \brief Function to solve the service solveListUsers
* \fn    int solveListUsers(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListUsers(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* option = NULL;
  std::string listUsersSerialized = "";
  std::string empty = "";
  std::string errorInfo;
  int mapperkey;
  Mapper* mapper;
  std::string cmd;


  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &option, NULL);

  SessionServer sessionServer  = SessionServer(std::string(sessionKey));
  ListUsersServer queryUsers(std::string(option), sessionServer);

  try {

    UMS_Data::ListUsers_ptr listUsers  = queryUsers.list();

    //To save the last connection on the database
    sessionServer.saveConnection();

    //MAPPER CREATION
    mapper = MapperRegistry::getInstance()->getMapper(UMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_list_users");
    mapper->code(std::string(option), mapperkey);
    cmd = mapper->finalize(mapperkey);

    //COMMAND REGISTRATION
    CommandServer commandServer = CommandServer(cmd, sessionServer);
    commandServer.record(UMS);

    const char* name = "listUsers";
    ::ecorecpp::serializer::serializer _ser(name);
    listUsersSerialized =  _ser.serialize(listUsers);

    //OUT Parameters
    diet_string_set(diet_parameter(pb,2), strdup(listUsersSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (VishnuException& e) {
      errorInfo =  e.buildExceptionString();
      //OUT Parameters
      diet_string_set(diet_parameter(pb,2), strdup(listUsersSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;

}

/**
* \brief Function to solve the service solveListMachines
* \fn int solveListMachines(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListMachines(diet_profile_t* pb) {

  return solveGenerique<UMS_Data::ListMachineOptions, UMS_Data::ListMachines, ListMachinesServer>(pb);
}

/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListLocalAccount(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb) {

  return solveGenerique<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts, ListLocalAccountsServer>(pb);
}

/**
* \brief Function to solve the service solveListOptions
* \fn int solveListOptions(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb) {

  return solveGenerique<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues, ListOptionsValuesServer>(pb);
}
/**
* \brief Function to solve the service solveListHistoryCmd
* \fn int solveListHistoryCmd(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb) {

  return solveGenerique<UMS_Data::ListCmdOptions, UMS_Data::ListCommands, ListCommandsServer>(pb);
}
/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListSessions(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb) {

  return solveGenerique<UMS_Data::ListSessionOptions, UMS_Data::ListSessions, ListSessionsServer>(pb);
}


/**
* \brief Function to solve the service solveRestore
* \fn    int solveRestore(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveRestore(diet_profile_t* pb) {

  char *sqlcode = NULL;
  std::string errorInfo;
  diet_string_get(diet_parameter(pb,0), &sqlcode, NULL);

  DbFactory factory;
  try {
  Database* db = factory.getDatabaseInstance();
  db->process(sqlcode);
  }
  catch (VishnuException& e) {
    errorInfo =  e.buildExceptionString();
  }
return 0;
}
