/**
* \file internalApi.cpp
* \brief This file implements the internal api of UMS
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011 
*/

#include "internalApi.hpp"
#include "utilServer.hpp"

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
  std::cout<<"userID:"<<userId<<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"password:"<<password<<std::endl;
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  std::cout<<"clientKey:"<<clientKey<<std::endl;
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  std::cout<<"clientHostname:"<<clientHostname<<std::endl;
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
    std::cout<<"sessionKey:"<<sessionServer.getData().getSessionKey()<<std::endl;
    
    //OUT Parameters
    diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
    
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  std::cout<<"userID:"<<userId<<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"password:"<<password<<std::endl;
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  std::cout<<"clientKey:"<<clientKey<<std::endl;
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  std::cout<<"clientHostname:"<<clientHostname<<std::endl;
  diet_string_get(diet_parameter(pb,4), &sessionId, NULL);
  std::cout<<"sessionId:"<<sessionId<<std::endl;
  
  UserServer userServer = UserServer(std::string(userId), std::string(password));
  MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
  
  
  SessionServer sessionServer = SessionServer(std::string(""));
  
  sessionServer.getData().setSessionId(std::string(sessionId)); 
  std::cout<<"sessionId dans SessionServer:"<<sessionServer.getData().getSessionId()<<std::endl;
 
  try {   
      sessionServer.reconnect(userServer, machineClientServer, std::string(sessionId));
       //OUT Parameters
      diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
    
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;	
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
  
   //IN Parameter
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  try {
    sessionServer.close();
     //OUT Parameter
    diet_string_set(diet_parameter(pb,1), strdup(empty.c_str()), DIET_VOLATILE);
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &userSerialized, NULL);
  std::cout<<"User:"<< userSerialized <<std::endl;
 
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
       //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &userSerialized, NULL);
  std::cout<<"User:"<< userSerialized <<std::endl;
 
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &userId, NULL);
  std::cout<<"User:"<< userId <<std::endl;
 
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);
  
  UMS_Data::User user;
  user.setUserId(userId);
  
  try {
    userServer.init();
    userServer.deleteUser(user);
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
     //OUT parameter
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  std::cout<<"sessionKey:"<< userId <<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"oldPassword:"<< password <<std::endl;
  diet_string_get(diet_parameter(pb,2), &newPassword, NULL);
  std::cout<<"newPassword:"<< newPassword <<std::endl;
  
  UserServer userServer = UserServer(std::string(userId), std::string(password));
  
  try { 
    userServer.init();
    userServer.changePassword(std::string(newPassword));
    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
       //OUT Parameter
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
} 

/**
#include <core/specs/exception/UMS.hh>
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &userId, NULL);
  std::cout<<"User:"<< userId <<std::endl;
 
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UserServer userServer = UserServer(sessionServer);
  
  UMS_Data::User user;
  user.setUserId(userId);
  
  try {
    userServer.init();
    userServer.resetPassword(user);
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
       //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &machineSerialized, NULL);
  std::cout<<"Machine:"<< machineSerialized <<std::endl;
 
  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  
  //Parse the model
  ecorecpp::parser::parser parser;
  Machine_ptr machine = parser.load(std::string(machineSerialized))->as< Machine >();
  MachineServer machineServer = MachineServer(machine, sessionServer);
  
  try {   
    machineServer.add();
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
       //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &machineSerialized, NULL);
  std::cout<<"Machine:"<< machineSerialized <<std::endl;
 
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  std::cout<<"Machine:"<< machineId <<std::endl;
 
  UMS_Data::Machine* machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  MachineServer machineServer = MachineServer(machine, sessionServer);
  
  try {   
    machineServer.deleteMachine();
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  std::string publicKey;
  std::string errorInfo;
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &laccountSerialized, NULL);
  std::cout<<"Local Account:"<< laccountSerialized <<std::endl;
 
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
    std::cout << "Public ssh key" << localAccountServer.getPublicKey() << std::endl;
     //OUT Parameters
    diet_string_set(diet_parameter(pb,2), strdup(localAccountServer.getPublicKey().c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
      
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &laccountSerialized, NULL);
  std::cout<<"Local Account Dans update:"<< laccountSerialized <<std::endl;
 
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  std::string publicKey;
  std::string errorInfo;
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &userId, NULL);
  std::cout<<"userId:"<< userId <<std::endl;
  diet_string_get(diet_parameter(pb,2), &machineId, NULL);
  std::cout<<"machineId:"<< machineId <<std::endl;
  
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  UMS_Data::LocalAccount *localAccount = new UMS_Data::LocalAccount();
  localAccount->setUserId(userId);
  localAccount->setMachineId(machineId);
  
  LocalAccountServer localAccountServer = LocalAccountServer(localAccount, sessionServer);
  
  try {
    localAccountServer.deleteLocalAccount();
     //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameter
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  ConfigurationServer configurationServer = ConfigurationServer(sessionServer);
  
  try {
  
  configurationServer.save();
  const char* name = "ConfigurationSave";
  ::ecorecpp::serializer::serializer _ser(name);
  configurationSerialized =  _ser.serialize(configurationServer.getData());
  
   //OUT Parameters
  diet_string_set(diet_parameter(pb,1), strdup(configurationSerialized.c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
      
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &configurationSerialized, NULL);
  std::cout<<"configuration:"<< configurationSerialized <<std::endl;
  
  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
  
  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  
  //Parse the model
  ecorecpp::parser::parser parser;
  UMS_Data::Configuration_ptr configuration = parser.load(std::string(configurationSerialized))->as< UMS_Data::Configuration >();
  
  ConfigurationServer configurationServer = ConfigurationServer(configuration, sessionServer);
  
  try {
    configurationServer.restore();
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);
  std::cout<<"options:"<< optionValueSerialized <<std::endl;
  
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout<<"sessionKey:"<< sessionKey <<std::endl;
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);
  std::cout<<"options:"<< optionValueSerialized <<std::endl;
  
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
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
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

  std::cout << "=================Solve userList =================" << std::endl;
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout << "=========sessionKey=" << sessionKey << "=============" << std::endl;
  diet_string_get(diet_parameter(pb,1), &optionValueSerialized, NULL);
  std::cout << "=========optionValueSerialized=" << std::endl;
  std::cout << optionValueSerialized  << std::endl;

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
    const char* name = "list";
    ::ecorecpp::serializer::serializer _ser(name);
    listSerialized =  _ser.serialize(list);
    
     //OUT Parameter
    diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
      
      //OUT Parameter
      diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

 std::cout << " done" << std::endl;

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

  std::cout << "=================Solve userList =================" << std::endl;
  
   //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  std::cout << "=========sessionKey=" << sessionKey << "=============" << std::endl;
  diet_string_get(diet_parameter(pb,1), &option, NULL);
  std::cout << "=========option=" << std::endl;
  std::cout << option  << std::endl;

  // Parse the model
  SessionServer sessionServer  = SessionServer(std::string(sessionKey));
  QueryUsers queryUsers(std::string(option), sessionServer);

  try {

    UMS_Data::ListUsers_ptr listUsers  = queryUsers.list();
    const char* name = "list";
    ::ecorecpp::serializer::serializer _ser(name);
    listUsersSerialized =  _ser.serialize(listUsers);
    
     //OUT Parameters
    diet_string_set(diet_parameter(pb,2), strdup(listUsersSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);

  } catch (SystemException& e) {
      errorInfo =  utilServer::buildExceptionString(e);
      std::cout << "errorInfo: " << errorInfo <<std::endl;
      
       //OUT Parameters
      diet_string_set(diet_parameter(pb,2), strdup(listUsersSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

 std::cout << " done" << std::endl;

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
 
  return solveGenerique<UMS_Data::ListMachineOptions, UMS_Data::ListMachines, QueryMachines>(pb);
}

/**
* \brief Function to solve the service solveListLocalAccount 
* \fn int solveListLocalAccount(diet_profile_t*& pb) 
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb) {
 
  return solveGenerique<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts, QueryLocalAccounts>(pb); 
}

/**
* \brief Function to solve the service solveListOptions 
* \fn int solveListOptions(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb) {

 return solveGenerique<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues, QueryOptions>(pb); 
}
/**
* \brief Function to solve the service solveListHistoryCmd 
* \fn int solveListHistoryCmd(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb) {

 return solveGenerique<UMS_Data::ListCmdOptions, UMS_Data::ListCommands, QueryCommands>(pb);
}
/**
* \brief Function to solve the service solveListLocalAccount 
* \fn int solveListSessions(diet_profile_t*& pb) 
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb) {
 return solveGenerique<UMS_Data::ListSessionOptions, UMS_Data::ListSessions, QuerySessions>(pb);
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
  db->startTransaction(sqlcode);
  db->commit();
  //db->endTransaction();
  }
   
  catch (SystemException& e) {
    errorInfo =  utilServer::buildExceptionString(e);
    std::cout << "errorInfo: " << errorInfo <<std::endl;
  }
 return 0; 
}
