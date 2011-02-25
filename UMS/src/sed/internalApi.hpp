/**
 * \file internalApi.hpp
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _INTERNALAPI_H_
#define _INTERNALAPI_H_

#include <exception>
#include "VishnuException.hpp"
#include "SystemException.hpp"
#include "DIET_server.h"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "ConfigurationServer.hpp"
#include "QueryServer.hpp"
#include "ListUsersServer.hpp"
#include "ListMachinesServer.hpp"
#include "ListLocalAccountsServer.hpp"
#include "ListOptionsValuesServer.hpp"
#include "ListCommandsServer.hpp"
#include "ListSessionsServer.hpp"
#include "CommandServer.hpp"
#include "UMSMapper.hpp"
#include "MapperRegistry.hpp"

/**
* \brief Function to solve the service sessionConnect
* \fn    int solveSessionConnect(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionConnect(diet_profile_t* pb);
/**
* \brief Function to solve the service sessionReconnect
* \fn    int solveSessionReconnect(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionReconnect(diet_profile_t* pb);
/**
* \brief Function to solve the service sessionClose
* \fn    int solveSessionClose(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionClose(diet_profile_t* pb);
/**
* \brief Function to solve the service userCreate
* \fn    int solveUserCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserUpdate
* \fn    int solveUserUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserDelete
* \fn    int solveUserDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserPasswordChange
* \fn    int solveUserPasswordChange(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordChange(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserPasswordReset
* \fn    int solveUserPasswordReset(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordReset(diet_profile_t* pb);

/**
* \brief Function to solve the service solveMachineCreate
* \fn    int solveMachineCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveMachineUpdate
* \fn    int solveMachineUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveMachineDelete
* \fn    int solveMachineDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountCreate
* \fn    int solveLocalAccountCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountUpdate
* \fn    int solveLocalAccountUpdate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountDelete
* \fn    int solveLocalAccountDelete(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountDelete(diet_profile_t* pb);

/**
* \brief Function to solve the service solveConfigurationSave
* \fn    int solveConfigurationSave(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveConfigurationSave(diet_profile_t* pb);
/**
* \brief Function to solve the service solveConfigurationRestore
* \fn    int solveConfigurationRestore(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveConfigurationRestore(diet_profile_t* pb);
/**
* \brief Function to solve the service solveOptionValueSet
* \fn    int solveOptionValueSet(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveOptionValueSet(diet_profile_t* pb);
/**
* \brief Function to solve the service solveOptionValueSetDefault
* \fn    int solveOptionValueSetDefault(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveOptionValueSetDefault(diet_profile_t* pb);
/**
* \brief Function to solve the service solveGenerique
* \fn    int solveGenerique(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListUsers
* \fn int solveListUsers(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListUsers(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListMachines
* \fn int solveListMachines(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListMachines(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListLocalAccount(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListOptions
* \fn int solveListOptions(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListHistoryCmd
* \fn int solveListHistoryCmd(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListLocalAccount
* \fn int solveListSessions(diet_profile_t*& pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb);
/**
* \brief Function to solve the service solveRestore
* \fn    int solveRestore(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveRestore(diet_profile_t* pb);
#endif // SERVERUMS
