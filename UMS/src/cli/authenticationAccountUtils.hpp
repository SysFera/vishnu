/**
 * \file authenticationAccountUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU add
 * update and delete authentication account commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef AUTHENTICATIONACCOUNTUTILS_HPP
#define AUTHENTICATIONACCOUNTUTILS_HPP


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


//Boost Headers

#include<boost/function.hpp>




class Options;

/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */


typedef  boost::function1<void,std::string> StringcallBackType;



/**
 * \brief To build options for the VISHNU system authentication commands
 * \param pgName : The name of the command
 * \param dietConfig : Represents the VISHNU config file
 * \param fAuthSytemId : the identifier of the user-authentication system
 * \param fUserId :represents an admin option to add a local user-authentication configuration of a specific user
 * \param fAcLogin : represents the login of the user on the associated user-authentication system
 * \param type : represents the type (if add or update)
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options> makeAuthAccountOptions(std::string pgName,std::string & dietConfig,StringcallBackType& fAuthSytemId,StringcallBackType& fUserId, StringcallBackType& fAcLogin, int type=0);

#endif



