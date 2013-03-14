/**
 * \file reconnect.cpp
 * This file defines the VISHNU reconnect command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <stdlib.h>                     // for exit
#include <unistd.h>                     // for getppid
#include <boost/program_options/errors.hpp>  // for error, etc
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <string>                       // for allocator, string, etc

#include "Options.hpp"                  // for Options, ::CONFIG, ::HIDDEN
#include "Session.hpp"                  // for Session
#include "UserException.hpp"
#include "VishnuException.hpp"          // for VishnuException, VISHNU_OK
#include "api_ums.hpp"                  // for reconnect, vishnuInitialize
#include "cliUtil.hpp"                  // for env_name_mapper, errorUsage, etc
#include "common.hpp"                   // for REQUIREDPARAMMSG
#include "connectUtils.hpp"             // for makeConnectOptions
#include "daemon_cleaner.hpp"           // for cleaner
#include "sessionUtils.hpp"             // for storeLastSession
#include "utilVishnu.hpp"               // for takePassword

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]) {
  string userId;
  string password;
  string sessionId;
  string configFile;

  /********* Out parameters     *****************/
  UMS_Data::Session session;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,0, configFile,CONFIG);

  opt->add("sessionId,s",
           "represents the identifier of the session",
           HIDDEN,
           sessionId,1);

  opt->setPosition("sessionId",1);

  opt->add("password,w","To give the password ",CONFIG,password );

  try {
    /**************  Parse to retrieve option values  ********************/
    opt->parse_cli(ac,av);
    opt->parse_env(env_name_mapper());
    opt->notify();


    /********  Process **************************/
    checkVishnuConfig(*opt);
    if ( opt->count("help")){
      helpUsage(*opt);
      exit(VISHNU_OK);
    }

    //Fix me
    if(0==opt->count("password") && 1==opt->count("userId")){
      password= vishnu::takePassword("password: ");
    } else if (1==opt->count("password") && 0==opt->count("userId")){
      errorUsage(av[0], "missing parameter: userId ");
      return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
    }


    /************** Call UMS reconnect service *******************************/
    cleaner(const_cast<char*>(configFile.c_str()), ac, av);// lauch the daemon cleaner if it is not already running

    // initializing VISHNU
    if (vishnuInitialize(const_cast<char*>(configFile.c_str()), ac, av)) {
      errorUsage(av[0],dietErrorMsg,EXECERROR);
      return  ERRCODE_CLI_ERROR_DIET ;
    }

    reconnect(userId, password, sessionId, session);

    // store the session information
    //storeLastSession(session.getSessionKey(),session.getClosePolicy(),getppid());
    storeLastSession(session,getppid());
  }  // End of try bloc

  catch(po::required_option& e){//  a required parameter is missing

    usage(*opt,"[options] sessionId",REQUIREDPARAMMSG);

    return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
  }
  // {{RELAX<CODEREDUCER> The error handling is the same in all command
  catch(po::error& e){ // catch all other bad parameter errors
    errorUsage(av[0], e.what());
    return ERRCODE_INVALID_PARAM;
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    return e.getMsgI() ;
  } catch(std::exception& e){//// catch all other command line runtime error
    errorUsage(av[0], e.what());
    return ERRCODE_CLI_ERROR_RUNTIME;
  }

  return 0;
  // }}RELAX<CODEREDUCER>
}// end of main
