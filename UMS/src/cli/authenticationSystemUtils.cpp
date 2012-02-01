/**
 * \file AuthenticationSystemUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU add
 * update and delete authentication commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include<iostream>
#include "authenticationSystemUtils.hpp"
#include "Options.hpp"
#include <string>
using namespace std;


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
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options> makeAuthSystemOptions(std::string pgName,std::string & dietConfig,StringcallBackType& fName,StringcallBackType& fURI, StringcallBackType& fAuthLogin,StringcallBackType& fAuthPassword,EncryptioncallBackType&fUserPasswordEncryption,AuthcallBackType& fType, std::string& ldapBase,int type){


  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);


  Group_type group=CONFIG;


  if(type){// type =0 for "update function" and type=1 for "add function"

    group=HIDDEN;
  }

  opt->add("name,n",
      "corresponds to the user-authentication system’s",
      group,
      fName,
      type);

  if (type){
    opt->setPosition("name",1);
  }

  opt->add("URI,i",
      "the URI of the user-authentication systems (by"
      "the form host:port for LDAP)",
      group,
      fURI,
      type);

  if (type){
    opt->setPosition("URI",1);
  }

  opt->add("authLogin,u",
      "the login used to connect to the"
      "user-authentication system",
      group,
      fAuthLogin,
      type);

  if (type){
    opt->setPosition("authLogin",1);
  }

  opt->add("authPassword,w",
      "the password used to connect to the"
      "user-authentication system",
      group,
      fAuthPassword,
      type);

  if (type){
    opt->setPosition("authPassword",1);
  }


  opt->add("userPasswordEncryption,e",
      "represents the encryption method used to encrypt"
      "user’s password",
      group,
      fUserPasswordEncryption,
      type);

  if (type){
    opt->setPosition("userPasswordEncryption",1);
  }


  opt->add("type,t",
      "represents the type of the user-authentication systems",
      group,
      fType,
      type);

  if (type){
    opt->setPosition("type",1);
  }

  opt->add("ldapBase,b",
      "is an option for user-authentication system based"
      "on LDAP which specifies the DN of the root entry",
      CONFIG,
      ldapBase);


  return opt;

}






