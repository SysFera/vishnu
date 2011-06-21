/**
 * \file machineUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU add
 * update and delte commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hpp"

#include "connectUtils.hpp"
#include "localAccountUtils.hpp"
#include<iostream>
using namespace std;


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
                                              std::string& sshPublicKeyPath, StringcallBackType& fMachineDescription,int type){

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
           "The name of the machine",
           group,
           fName,
           type);

  if (type){
    opt->setPosition("name",1);
  }

  opt->add("site,s",
           "The location of the machine",
           group,
           fSite,
           type);

  if (type){
    opt->setPosition("site",1);
  }

  opt->add("language,l",
           "The language in which the description of the machine has been done",
           group,
           fLanguage,
           type);

  if (type){
    opt->setPosition("language",1);
  }

  opt->add("sshPublicKeyFile,k",
           "The the path to the SSH public key used by VISHNU to access local user accounts",
           group,
           sshPublicKeyPath,
           type);

  if(type){

    opt->setPosition("sshPublicKeyFile",1);
  }

  opt->add("machineDescription,d",
           "The description of the machine",
           group,
          fMachineDescription,
          type);

  if(type){
    opt->setPosition("machineDescription",1);
  }




  return opt;



}






