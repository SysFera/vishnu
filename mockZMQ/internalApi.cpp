/**
 * FAKE FILE
* \file UMS/src/sed/internalApi.cpp
* \brief This file implements the internal api of UMS
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "internalApi.hpp"

using namespace vishnu;

/**
* \brief Function to solve the service sessionConnect
* \fn    void solveSessionConnect(diet_profile_t* pb);
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionConnect(diet_profile_t* pb) {
  std::cout << "executing connect" << std::endl;
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
  std::cout << "executing reconnect" << std::endl;
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
  std::cout << "executing close" << std::endl;
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
  std::cout << "executing createuser" << std::endl;
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
  std::cout << "executing updateuser" << std::endl;
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
  std::cout << "executing deleteuser" << std::endl;
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
  std::cout << "executing changepwd" << std::endl;
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
  std::cout << "executing resetpwd" << std::endl;
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
  std::cout << "executing createmachine" << std::endl;
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
  std::cout << "executing updatemachine" << std::endl;
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
  std::cout << "executing deletemachine" << std::endl;
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
  std::cout << "executing locacccreate" << std::endl;
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
  std::cout << "executing locaccupdate" << std::endl;
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
  std::cout << "executing locaccdelete" << std::endl;
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
  std::cout << "executing configsave" << std::endl;
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
  std::cout << "executing configrestore" << std::endl;
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
  std::cout << "executing opvalset" << std::endl;
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
  std::cout << "executing opvalsetdefault" << std::endl;
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
  std::cout << "executing listuser" << std::endl;
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
  std::cout << "executing listmachine" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListLocalAccount(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb) {
  std::cout << "executing listlocacc" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveListOptions
* \fn int solveListOptions(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb) {
  std::cout << "executing listop" << std::endl;
  return 0;
}
/**
* \brief Function to solve the service solveListHistoryCmd
* \fn int solveListHistoryCmd(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb) {
  std::cout << "executing listcmd" << std::endl;
  return 0;
}
/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListSessions(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb) {
  std::cout << "executing listsession" << std::endl;
  return 0;
}


/**
* \brief Function to solve the service solveRestore
* \fn    int solveRestore(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveRestore(diet_profile_t* pb) {
  std::cout << "executing restore" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthCreate
* \fn    int solveSystemAuthCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthCreate(diet_profile_t* pb) {
  std::cout << "executing sysauthcreate" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthUpdate
* \fn    int solveSystemAuthUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthUpdate(diet_profile_t* pb) {
  std::cout << "executing sysauthupdate" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthDelete
* \fn    int solveSystemAuthDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthDelete(diet_profile_t* pb) {
  std::cout << "executing sysauthdelete" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveSystemAuthList
* \fn    int solveSystemAuthList(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthList(diet_profile_t* pb) {
  std::cout << "executing sysauthlist" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthCreate
* \fn    int solveAccountAuthCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthCreate(diet_profile_t* pb) {
  std::cout << "executing authacccreate" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthUpdate
* \fn    int solveAccountAuthUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthUpdate(diet_profile_t* pb) {
  std::cout << "executing authaccupdate" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthDelete
* \fn    int solveAccountAuthDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthDelete(diet_profile_t* pb) {
  std::cout << "executing authaccdelete" << std::endl;
  return 0;
}

/**
* \brief Function to solve the service solveAccountAuthList
* \fn    int solveAccountAuthList(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthList(diet_profile_t* pb) {
  std::cout << "executing authacclist" << std::endl;
  return 0;
}
