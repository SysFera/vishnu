/**
 * \file machineUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU add
 * update and delte commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef MACHINEUTILS__HH
#define MACHNINEUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */


typedef  boost::function1<void,std::string> StringcallBackType;


/**
 * \brief To build options for the VISHNU machine commands
 * \param pgName : The name of the command
 * \param fName : The machine name option callback
 * \param dietConfig : Represents the VISHNU config file
 * \param fSite : The site name option callback
 * \param fLanguage : The language option callback
 * \param sshPublicKeyPath : The user ssh key path option
 * \param fMachineDescription : The machine description option callback
 * \param type : Indicates if it is an add or an update command, 0 means update, and 1 means add
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options> makeMachineOptions(std::string pgName,StringcallBackType& fName,std::string & dietConfig,
                                              StringcallBackType & fSite, StringcallBackType& fLanguage,
                                              std::string& sshPublicKeyPath, StringcallBackType& fMachineDescription,int type=0);





#endif



