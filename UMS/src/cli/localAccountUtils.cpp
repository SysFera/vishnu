/**
 * \file localAccountUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU local account commands
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "localAccountUtils.hpp"
#include "Options.hpp"
#include "connectUtils.hpp"

using namespace std;

/**
 * \brief To build options for the VISHNU local account commands
 * \param pgName : The name of the command
 * \param fUserId: The user identifier option callback
 * \param configFile: Represents the VISHNU config file
 * \param fMachineId: The machine identifier option callback
 * \param fAcLogin: The user login option callback
 * \param fHomeDirectory: The user home directory option callback
 * \param type: Indicates if it is an add or an update command, 0 means update, and 1 means add
 * \return The description of all options allowed by the command
 */

boost::shared_ptr<Options>
makeLocalAccountOptions(std::string pgName,
                        StringcallBackType& fUserId,
                        std::string & configFile,
                        StringcallBackType & fMachineId,
                        StringcallBackType& fAcLogin,
                        StringcallBackType & fHomeDirectory,
                        int type){

  boost::shared_ptr<Options> opt=makeConnectOptions(pgName,fUserId,1,configFile);

  opt->setPosition("userId", 1);

  opt->add("machineId,m",
           "The identifier of the target machine",
           CONFIG,
           fMachineId,
           type);

  opt->add("acLogin,l",
           "User login on the targer machine",
           CONFIG,
           fAcLogin,
           type);

  opt->add("homeDirectory,d",
           "The user home on the target machine",
           CONFIG,
           fHomeDirectory,
           type);

  return opt;
}
