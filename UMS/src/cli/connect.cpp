/**
 * \file connect.cpp
 * This file defines the VISHNU connect command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "daemon_cleaner.hpp"
#include "utilVishnu.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string userId;

  std::string password;


  /********** EMF Data ****************************/


  UMS_Data::ConnectOptions connectOpt;



  /******** Callback functions ******************/

  boost::function1<void,UMS_Data::SessionCloseType> fClosePolicy( boost::bind(&UMS_Data::ConnectOptions::setClosePolicy,boost::ref(connectOpt),_1));
  boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ConnectOptions::setSessionInactivityDelay,boost::ref(connectOpt),_1));
  boost::function1<void,string> fSubstituteUserId( boost::bind(&UMS_Data::ConnectOptions::setSubstituteUserId,boost::ref(connectOpt),_1));

  /*********** Out parameters *********************/


  UMS_Data::Session session;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

  opt->setPosition("userId",-1);

  opt->add("sessionInactivityDelay,d", "The session inactivity delay",CONFIG,fSessionInactivityDelay);


  opt->add("substituteUserId,s","The substitute user identifier",CONFIG,fSubstituteUserId);

  opt->add("closePolicy,p","for closing session automatically",CONFIG,fClosePolicy );
  
  opt->add("password,w","To give the password ",CONFIG,password );

  try{


    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/

    checkVishnuConfig(*opt);

    //Fix me

    if(0==opt->count("password")){

    password= vishnu::takePassword("password: ");
    
    }

    /************** Call UMS connect service *******************************/

    cleaner(const_cast<char*>(dietConfig.c_str()), ac, av);// lauch the daemon cleaner if it is not already running

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
    
      errorUsage(av[0],dietErrorMsg,EXECERROR);
    
      return  CLI_ERROR_DIET ;
    }


    connect(userId,password, session, connectOpt);// call the api extern connect service to get a session key

    vishnuFinalize();

    storeLastSession(session.getSessionKey(),session.getClosePolicy(),getppid()); // store sessionKey into $HOME/.vishnu/sessions

    printSuccessMessage();


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] userId ",requiredParamMsg);
    
    return CLI_ERROR_MISSING_PARAMETER;
  }
  
  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());
    
    return CLI_ERROR_INVALID_PARAMETER;
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
    
    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


