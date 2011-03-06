/**
 * \file list_sessions.cpp
 * This file defines the VISHNU list sessions command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "listHistoryCmdUtils.hpp"
#include<boost/bind.hpp>
#include "sessionUtils.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string startDateOption;
  
  std::string endDateOption;

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListSessions listSession;

  UMS_Data::ListSessionOptions listOptions;

  /**************** Callback functions *************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListSessionOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fSessionId( boost::bind(&UMS_Data::ListSessionOptions::setSessionId,boost::ref(listOptions),_1));

  boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ListSessionOptions::setSessionInactivityDelay,boost::ref(listOptions),_1));

  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListSessionOptions::setMachineId,boost::ref(listOptions),_1));

  boost::function1<void,UMS_Data::SessionCloseType> fSessionClosePolicy( boost::bind(&UMS_Data::ListSessionOptions::setSessionClosePolicy,boost::ref(listOptions),_1));
  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::ListSessionOptions::setStatus,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListHistoryCmdOptions(av[0],fUserId, dietConfig, fSessionId, startDateOption, endDateOption);


  opt->add("status,t",
           "specifies the type of the sessions which will be\n"
           "listed (INACTIVE or ACTIVE)",
           CONFIG,
           fStatus);

  opt->add("sessionClosePolicy,p",
           "Specifies the closure mode of the sessions which\n"
           "will be listed (CLOSE_ON_TIMEOUT or CLOSE_ON_DISCONNECT)",
           CONFIG,
           fSessionClosePolicy);

  opt->add("sessionInactivityDelay,d",
           "specifies the inactivity delay in seconds\n"
           "of the sessions which will be listed",
           CONFIG,
           fSessionInactivityDelay);

  opt->add("machineId,m",
           "allows the user to list sessions\n"
           "opened on a specific machine",
           CONFIG,
           fMachineId);


  try{
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    bool isEmpty=opt->empty();//if no value was given in the command line

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/


    if (opt->count("adminListOption")){

      listOptions.setAdminListOption(true);
    }


    checkVishnuConfig(*opt);

    if ( opt->count("help")){

      helpUsage(*opt,"[options]  ");

      return 0;
    }


    //convert the date in long format 
    
    if(opt->count("startDateOption")){
      listOptions.setStartDateOption(string_to_time_t(startDateOption));
    }

    if(opt->count("endDateOption")){
      listOptions.setEndDateOption(string_to_time_t(endDateOption));
    }


    /************** Call UMS list sessions service *******************************/


    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
    
      errorUsage(av[0],"DIET initialization failed !",EXECERROR);

            return  CLI_ERROR_DIET ;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      listSessions(sessionKey,listSession,listOptions);

    }


    // Display the list
    if(isEmpty|| (opt->count("adminListOption"))) {
      cout << listSession << std::endl;
    }
    else {
      for(int i = 0; i < listSession.getSessions().size(); i++) {
        cout << listSession.getSessions().get(i) << endl;
      }
    }

    printSuccessMessage();
  }// End of try bloc

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


