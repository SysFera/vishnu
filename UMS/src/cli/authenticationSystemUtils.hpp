/**
 * \file authenticationSystemUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU add
 * update and delete authentication commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef AUTHENTICATIONSYSTEMUTILS_HPP
#define AUTHENTICATIONSYSTEMUTILS_HPP


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
 * \brief A convenient function type taking a EncryptionMethod parameter and
 * without return value
 */


typedef  boost::function1<void,UMS_Data::EncryptionMethod> EncryptioncallBackType;


/**
 * \brief A convenient function type taking a AuthType parameter and
 * without return value
 */


typedef  boost::function1<void,UMS_Data::AuthType> AuthcallBackType;

/**
 * \brief A convenient function type taking a StatusType parameter and
 * without return value
 */


typedef  boost::function1<void,UMS_Data::StatusType> StatuscallBackType;





/**
 * \brief To build options for the VISHNU system authentication commands
 * \param pgName : The name of the command
 * \param dietConfig : Represents the VISHNU config file
 * \param fName : The user-authentication system’s name option callback
 * \param fURI : The URI of the user-authentication systems (by
 * the form host:port for LDAP) option callback
 * \param fAuthLogin : The login used to connect to the user-authentication system option callback
 * \param fAuthPassword : The e password used to connect to the user-authentication system option callback
 * \param fUserPasswordEncryption  : The user encryption password option callback
 * \param fType : user-authentication systems option callback
 * \param fLdapBase : option for user-authentication system based  on LDAP which specifies the DN of the root entry
 * \param type : represents the type of operation to perform (add or update)
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options> makeAuthSystemOptions(std::string pgName,std::string & dietConfig,StringcallBackType& fName,StringcallBackType& fURI, StringcallBackType& fAuthLogin,StringcallBackType& fAuthPassword, EncryptioncallBackType&fUserPasswordEncryption,AuthcallBackType& fType, StringcallBackType& fLdapBase,int type=0);

#endif



