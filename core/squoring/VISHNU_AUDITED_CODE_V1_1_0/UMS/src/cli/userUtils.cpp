/**
 * \file userUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU user commands 
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "Options.hpp"

#include "connectUtils.hpp"
#include "userUtils.hpp"
#include<iostream>
using namespace std;

/**
 * \brief To build options for the VISHNU user commands
 * \param pgName : The name of the command
 * \param dietConfig: Represents the VISHNU config file
 * \param fPrivilege: The privilege option callback
 * \param ffirstname: The user first name option callback
 * \param fLastName: The user last name option callback
 * \param fEmail: The user email option callback
 * \param type: Indicates if it is an add or an update command, 0 means update, and 1 means add
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options>
makeUserOptions(std::string pgName,std::string & dietConfig,
                privilegeCallBackType & fPrivilege, StringcallBackType& fFirstname,
                StringcallBackType& fLastname, StringcallBackType& fEmail, int type){


  boost::shared_ptr<Options> opt(new Options(pgName));

  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  Group_type group=CONFIG;

  if(type){// type =0 for "update function" and type=1 for "add function"

    group=HIDDEN;
  }


  opt->add("firstname,f",
           "The firstname of the user",
           group,
           fFirstname,
           type);



  if(type){

    opt->setPosition("firstname",1);

  }


  opt->add("lastname,l",
           "The lastname of the user",
           group,
           fLastname,
           type);

  if(type){
    opt->setPosition("lastname",1);

  }

  opt->add("privilege,p",
           "the privilege of the user (admin or simple user)",
           group,
           fPrivilege,
           type);


  if(type){
    opt->setPosition("privilege",1);
  }

  opt->add("email,m",
           "The email of the user",
           group,
           fEmail,
           type);

  if(type){
    opt->setPosition("email",1);

  }



  return opt;



}






