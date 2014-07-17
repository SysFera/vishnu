/**
 * \file connect.cpp
 * This file defines the VISHNU connect command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <stdlib.h>                     // for exit
#include <unistd.h>                     // for getppid
#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/program_options/errors.hpp>  // for error, etc
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <sstream>                      // for basic_stringbuf<>::int_type, etc
#include <string>                       // for string, allocator, etc

#include "ConnectOptions.hpp"           // for ConnectOptions
#include "Options.hpp"                  // for Options, ::CONFIG, ::ENV
#include "Session.hpp"                  // for Session
#include "UMSVishnuException.hpp"       // for ERRCODE_TEMPORARY_PASSWORD
#include "UserException.hpp"
#include "VishnuException.hpp"          // for VishnuException, VISHNU_OK
#include "api_ums.hpp"                  // for connect, vishnuInitialize
#include "cliUtil.hpp"                  // for errorUsage, ::EXECERROR, etc
#include "common.hpp"                   // for REQUIREDPARAMMSG
#include "connectUtils.hpp"             // for UMS_env_name_mapper, etc
#include "daemon_cleaner.hpp"           // for cleaner
#include "ecore_forward.hpp"            // for EInt
#include "sessionUtils.hpp"             // for storeLastSession
#include "utilVishnu.hpp"               // for takePassword

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


int
main(int ac, char* av[]) {
  /******* Parsed value containers ****************/
  std::string configFile;
  std::string userId;
  std::string password;
  std::string key;

  /********** EMF Data ****************************/
  UMS_Data::ConnectOptions connectOpt;

  /******** Callback functions ******************/
  boost::function1<void, UMS_Data::SessionCloseType> fClosePolicy(boost::bind(&UMS_Data::ConnectOptions::setClosePolicy, boost::ref(connectOpt),_1));
  boost::function1<void, int> fSessionInactivityDelay(boost::bind(&UMS_Data::ConnectOptions::setSessionInactivityDelay, boost::ref(connectOpt),_1));
  boost::function1<void, std::string> fSubstituteUserId(boost::bind(&UMS_Data::ConnectOptions::setSubstituteUserId, boost::ref(connectOpt),_1));

  /*********** Out parameters *********************/
  UMS_Data::Session session;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt =
    makeConnectOptions(av[0], userId, 0, configFile, CONFIG);

  opt->add("sessionInactivityDelay,d", "The session inactivity delay",
           CONFIG,fSessionInactivityDelay);
  opt->add("substituteUserId,s", "The substitute user identifier",
           CONFIG, fSubstituteUserId);
  opt->add("closePolicy,p", "for closing session automatically", ENV, fClosePolicy);
  opt->add("password,w", "To give the password ", CONFIG, password);

  opt->add("showKey,k",
           "An option for listing all VISHNU machines",
           CONFIG);

  try {
    /**************  Parse to retrieve option values  ********************/
    opt->parse_cli(ac, av);
    opt->parse_env(UMS_env_name_mapper());
    opt->notify();

    /********  Process **************************/
    checkVishnuConfig(*opt);
    if (opt->count("help")) {
      helpUsage(*opt);
      exit(VISHNU_OK);
    }

    //Fix me
    if ((0 == opt->count("password")) && (1 == opt->count("userId"))) {
      password= vishnu::takePassword("password: ");
    } else if ((1 == opt->count("password")) && (0 == opt->count("userId"))) {
      errorUsage(av[0], "missing parameter: userId ");
      return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
    }

    /************** Call UMS connect service *******************************/
// lauch the daemon cleaner if it is not already running
    cleaner(const_cast<char*>(configFile.c_str()), ac, av);

    // initializing vishnu
    if (vishnuInitialize(const_cast<char*>(configFile.c_str()), ac, av)) {
      errorUsage(av[0],communicationErrorMsg,EXECERROR);
      return  ERRCODE_CLI_ERROR_COMMUNICATION ;
    }

    connect(userId,password, session, connectOpt);// call the api extern connect service to get a session key

    storeLastSession(session,getppid()); // store sessionKey into $HOME/.vishnu/sessions
    std::cout << "sessionId: " << session.getSessionId() << "\n";
    if (opt->count("showKey")){
      std::cout << "sessionKey: " << session.getSessionKey() << "\n";
    }
  }  // End of try bloc


  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] ",REQUIREDPARAMMSG);

    return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
  }
  // {{RELAX<CODEREDUCER> The error handling is the same in all command

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return ERRCODE_INVALID_PARAM;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    // handle the temporary password
    if(ERRCODE_TEMPORARY_PASSWORD==e.getMsgI()){

      errorUsage(av[0],"Your password is temporary, use vishnu_change_password command to change it",EXECERROR);
    }

    else{

      std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

      errorUsage(av[0],msg,EXECERROR);
    }

    return e.getMsgI() ;

  }

  catch(std::exception& e){ // catch all std runtime error

    errorUsage(av[0], e.what());

    return ERRCODE_CLI_ERROR_RUNTIME;
  }

  return 0;

// }}RELAX<CODEREDUCER>

}// end of main
