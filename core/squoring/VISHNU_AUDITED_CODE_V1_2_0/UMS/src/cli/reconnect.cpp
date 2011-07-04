/**
 * \file reconnect.cpp
 * This file defines the VISHNU reconnect command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include "daemon_cleaner.hpp"
#include "utilVishnu.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


	string userId;

	string password;

	string sessionId;

	string dietConfig;

/********* Out parameters     *****************/

  UMS_Data::Session session;

  /**************** Describe options *************/


  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1, dietConfig);

  opt->setPosition("userId",1);


  opt->add("sessionId,s",
           "represents the identifier of the session",
           HIDDEN,
           sessionId,1);

  opt->setPosition("sessionId",1);



  try {

    /**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();


/********  Process **************************/

		checkVishnuConfig(*opt);


    //Fix me
    password=vishnu::takePassword("Password: ");


    /************** Call UMS reconnect service *******************************/

    cleaner(const_cast<char*>(dietConfig.c_str()), ac, av);// lauch the daemon cleaner if it is not already running  

    // initializing DIET
   
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);
    
      return  CLI_ERROR_DIET ;

    }


    reconnect(userId, password, sessionId, session);
   

    // store the session information

    storeLastSession(session.getSessionKey(),session.getClosePolicy(),getppid());
    
    printSuccessMessage();


  }// End of try bloc

  catch(po::required_option& e){//  a required parameter is missing

    usage(*opt,"[options] userId sessionId",requiredParamMsg);
   
    return CLI_ERROR_MISSING_PARAMETER;
  }

  // {{RELAX<CODEREDUCER> The error handling is the same in all command

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }


  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){//// catch all other command line runtime error

    errorUsage(av[0], e.what()) ;

    return CLI_ERROR_RUNTIME;
  }

  return 0;

  // }}RELAX<CODEREDUCER>

}// end of main


