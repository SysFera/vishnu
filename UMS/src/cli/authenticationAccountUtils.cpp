/**
 * \file authenticationAccountUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU add
 * update and delete authentication account commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include<iostream>
#include "authenticationAccountUtils.hpp"
#include "Options.hpp"
#include <string>
using namespace std;



/**
 * \brief To build options for the VISHNU system authentication commands
 * \param pgName : The name of the command
 * \param dietConfig : Represents the VISHNU config file
 * \param fAuthSystemId : the identifier of the user-authentication system
 * \param fUserId :represents an admin option to add a local user-authentication configuration of a specific user
 * \param fAcLogin : represents the login of the user on the associated user-authentication system
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options> makeAuthAccountOptions(std::string pgName,std::string & dietConfig,StringcallBackType& fAuthSystemId,StringcallBackType& fUserId, StringcallBackType& fAcLogin, int type) {


  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);


  Group_type group=CONFIG;


  if(type){// type =0 for "update function" and type=1 for "add function"

    group=HIDDEN;
  }

  opt->add("authSystemId",
      "the identifier of the user-authentication system",
      HIDDEN,
      fAuthSystemId,
      1);

    opt->setPosition("authSystemId",1);


  opt->add("acLogin,l",
      " represents the login of the user on the associated "
      "user-authentication system",
      group,
      fAcLogin,
      type);

  if (type){
    opt->setPosition("acLogin",1);
  }


  opt->add("userId,u",
      "represents an admin option to add a local"
      "user-authentication configuration of a specific user",
      CONFIG,
      fUserId);


  return opt;

}




