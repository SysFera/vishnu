/**
 * \file reconnect_m.cpp
 * This file defines the VISHNU reconnect command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <stdlib.h>
#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include "daemon_cleaner.hpp"
#include "utilVishnu.hpp"
#include "UserException.hpp"

using namespace std;
using namespace vishnu;


int main (int ac, char* av[]){

  int counter;

  string sessionId;

  string dietConfig;

  string userId;

  string password;

  /*********** In parameters **********************/

  UMS_Data::ListUsers listUsers;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,0, dietConfig,CONFIG);



  opt->add("sessionId,s",
           "represents the identifier of the session",
           HIDDEN,
           sessionId,1);

  opt->setPosition("sessionId",1);


  opt->add("password,w","To give the password ",CONFIG,password );

/********* Out parameters     *****************/

  UMS_Data::Session session;



  try {

    dietConfig = string(getenv("VISHNU_CONFIG_FILE"));
    if (ac<2) {
      helpUsage(*opt,"[option] ");
      return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
    }

    sessionId = string(av[1]);

    /************** Call UMS reconnect service *******************************/

    cleaner(const_cast<char*>(dietConfig.c_str()), ac, av);// lauch the daemon cleaner if it is not already running

    /********* Get all the couples user/pwd ******************/
    counter = 2;
    while (counter<ac) {
      string tmp(av[counter]);
      if (counter+1>=ac) {
        helpUsage(*opt,"[option] ");
        return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
      }
      UMS_Data::User_ptr user = ecoreFactory->createUser();
      user->setUserId(tmp);
      user->setPassword(string(av[counter+1]));
      listUsers.getUsers().push_back(user);
      counter += 2;
    }

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  ERRCODE_CLI_ERROR_DIET ;

    }

    reconnect(listUsers, sessionId, session);

    // store the session information

    storeLastSession(session,getppid());

    printSuccessMessage();


  }// End of try bloc

  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){//// catch all other command line runtime error

    errorUsage(av[0], e.what()) ;

    return ERRCODE_CLI_ERROR_RUNTIME;
  }

  return 0;

  // }}RELAX<CODEREDUCER>

}// end of main


