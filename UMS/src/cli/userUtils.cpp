#include "Options.hpp"

#include "connectUtils.hpp"
#include "userUtils.hpp"
#include<iostream>
using namespace std;

boost::shared_ptr<Options> makeUserOptions(std::string pgName,std::string & dietConfig,
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






