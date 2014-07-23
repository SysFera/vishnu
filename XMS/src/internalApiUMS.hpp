/**
 * \file UMS/src/sed/internalApi.hpp
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef _INTERNALAPI_H_
#define _INTERNALAPI_H_

#include <exception>
#include "VishnuException.hpp"
#include "SystemException.hpp"
#include "DIET_client.h"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
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
#include "AuthSystemServer.hpp"
#include "AuthAccountServer.hpp"
#include "ListAuthSystemsServer.hpp"
#include "ListAuthAccountsServer.hpp"

/**
* \brief Function to solve the service sessionConnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionConnect(diet_profile_t* pb);
/**
* \brief Function to solve the service sessionReconnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionReconnect(diet_profile_t* pb);
/**
* \brief Function to solve the service sessionClose
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSessionClose(diet_profile_t* pb);
/**
* \brief Function to solve the service userCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserPasswordChange
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordChange(diet_profile_t* pb);
/**
* \brief Function to solve the service solveUserPasswordReset
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveUserPasswordReset(diet_profile_t* pb);

/**
* \brief Function to solve the service solveMachineCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveMachineUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveMachineDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveMachineDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveLocalAccountDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveLocalAccountDelete(diet_profile_t* pb);

/**
* \brief Function to solve the service solveGenerique
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListUsers
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListUsers(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListMachines
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListMachines(diet_profile_t* pb);

/**
* \brief Function to solve the service solveListLocalAccount
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListLocalAccount(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListOptions
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListOptions(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListHistoryCmd
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListHistoryCmd(diet_profile_t* pb);
/**
* \brief Function to solve the service solveListLocalAccount
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveListSessions(diet_profile_t* pb);
/**
* \brief Function to solve the service solveRestore
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveRestore(diet_profile_t* pb);
/**
* \brief Function to solve the service solveSystemAuthCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveSystemAuthUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveSystemAuthDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveSystemAuthList
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveSystemAuthList(diet_profile_t* pb);
/**
* \brief Function to solve the service solveAccountAuthCreate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthCreate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveAccountAuthUpdate
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthUpdate(diet_profile_t* pb);
/**
* \brief Function to solve the service solveAccountAuthDelete
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthDelete(diet_profile_t* pb);
/**
* \brief Function to solve the service solveAccountAuthList
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int
solveAccountAuthList(diet_profile_t* pb);
/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveExport(diet_profile_t* pb);

#endif // SERVERUMS
