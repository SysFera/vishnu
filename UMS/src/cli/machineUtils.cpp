#include "Options.hpp"

#include "connectUtils.hpp"
#include "localAccountUtils.hpp"
#include<iostream>
using namespace std;

boost::shared_ptr<Options> makeMachineOptions(std::string pgName,StringcallBackType& fName,std::string & dietConfig,
                                              StringcallBackType & fSite, StringcallBackType& fLanguage,int type){

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


  return opt;



}






