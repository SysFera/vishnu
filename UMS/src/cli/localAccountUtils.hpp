/**
 * \file localAccountUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU local account commands
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef LOCALACCOUNTUTILS__HH
#define LOCALACCOUNTUTILS__HH


//VISHNU Headers

#include "common.hpp"

//Boost Headers

#include<boost/function.hpp>


class Options;

/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */

typedef
boost::function1<void,std::string> StringcallBackType;


/**
 * \brief To build options for the VISHNU local account commands
 * \param pgName : The name of the command
 * \param fUserId: The user identifier option callback
 * \param dietConfig: Represents the VISHNU config file
 * \param fMachineId: The machine identifier option callback
 * \param fAcLogin: The user login option callback
 * \param fSshKeyPath: The user ssh key path option callback
 * \param fHomeDirectory: The user home directory option callback
 * \param type: Indicates if it is an add or an update command, 0 means update, and 1 means add
 * \return The description of all options allowed by the command
 */

boost::shared_ptr<Options>
makeLocalAccountOptions(std::string pgName,StringcallBackType& fUserId,
                        std::string & dietConfig, StringcallBackType & fMachineId,
                        StringcallBackType& fAcLogin, StringcallBackType& fSshKeyPath,
                        StringcallBackType & fHomeDirectory, int type=0);






#endif



