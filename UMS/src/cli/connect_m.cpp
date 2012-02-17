/**
 * \file connect_m.cpp
 * This file defines the VISHNU connect command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "daemon_cleaner.hpp"
#include "utilVishnu.hpp"
#include "UMSVishnuException.hpp"
#include "UserException.hpp"
#ifdef WIN32
#include "OSIndependance.hpp"
#endif
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF Data ****************************/

  UMS_Data::ConnectOptions connectOpt;

  /*********** In parameters **********************/

  UMS_Data::ListUsers listUsers;
  
  
  /******** Callback functions ******************/



  boost::function1<void,UMS_Data::SessionCloseType> fClosePolicy( boost::bind(&UMS_Data::ConnectOptions::setClosePolicy,boost::ref(connectOpt),_1));
  boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ConnectOptions::setSessionInactivityDelay,boost::ref(connectOpt),_1));
  boost::function1<void,string> fSubstituteUserId( boost::bind(&UMS_Data::ConnectOptions::setSubstituteUserId,boost::ref(connectOpt),_1));

  /*********** Out parameters *********************/


  UMS_Data::Session session;


  /**************** Describe options *************/
    
  boost::shared_ptr< Options> opt(new Options(av[0]) );

  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);


  opt->add("sessionInactivityDelay,d", "The session inactivity delay",CONFIG,fSessionInactivityDelay);


  opt->add("substituteUserId,s","The substitute user identifier",CONFIG,fSubstituteUserId);

  opt->add("closePolicy,p","for closing session automatically",ENV,fClosePolicy );


  try{

    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(UMS_env_name_mapper());

    opt->notify();


    /********  Process **************************/

    checkVishnuConfig(*opt);
    if ( opt->count("help")){
      helpUsage(*opt,"[option] ");
      exit(VISHNU_OK);
    }


    /************** Call UMS connect service *******************************/

    cleaner(const_cast<char*>(dietConfig.c_str()), ac, av);// lauch the daemon cleaner if it is not already running

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  ERRCODE_CLI_ERROR_DIET ;
    }

    //connect(listUsers, session, connectOpt);// call the api extern connect service to get a session key

    vishnuFinalize();

#ifndef WIN32
    storeLastSession(session,getppid()); // store sessionKey into $HOME/.vishnu/sessions
#else
    DWORD cpid = GetCurrentProcessId();
    storeLastSession(session,(int)GetParentProcessID(cpid));
#endif    
    std::cout << "sessionId: " << session.getSessionId() << "\n";

    printSuccessMessage();


  }// End of try bloc


  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] ",requiredParamMsg);
    
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


