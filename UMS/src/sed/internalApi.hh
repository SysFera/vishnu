/**
 * \file internalApi.hh
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#ifndef _INTERNALAPI_H_
#define _INTERNALAPI_H_

#include <exception>
#include "SystemException.hh"
#include "DIET_server.h"
#include "UserServer.hh"
#include "SessionServer.hh"
#include "MachineClientServer.hh"

/**
* \brief Function to solve the service SessionConnect 
* \fn    void solveSessionConnect(diet_profile_t* pb);
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSessionConnect(diet_profile_t* pb);
/**
* \brief Function to solve the service SessionReconnect 
* \fn    void solveSessionReconnect
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSessionReconnect(diet_profile_t* pb);
/**
* \brief Function to solve the service SessionClose 
* \fn    virtual int createDatabase() = 0
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSessionClose(diet_profile_t* pb);  

#endif // SERVERUMS
