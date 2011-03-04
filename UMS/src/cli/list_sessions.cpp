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

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListSessions listSession;

  UMS_Data::ListSessionOptions listOptions;

  /**************** Callback functions *************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListSessionOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fSessionId( boost::bind(&UMS_Data::ListSessionOptions::setSessionId,boost::ref(listOptions),_1));

  boost::function1<void,long> fStartDateOption( boost::bind(&UMS_Data::ListSessionOptions::setStartDateOption,boost::ref(listOptions),_1));

  boost::function1<void,long> fEndDateOption( boost::bind(&UMS_Data::ListSessionOptions::setEndDateOption,boost::ref(listOptions),_1));

  boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ListSessionOptions::setSessionInactivityDelay,boost::ref(listOptions),_1));

  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListSessionOptions::setMachineId,boost::ref(listOptions),_1));

  boost::function1<void,UMS_Data::SessionCloseType> fSessionClosePolicy( boost::bind(&UMS_Data::ListSessionOptions::setSessionClosePolicy,boost::ref(listOptions),_1));
  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::ListSessionOptions::setStatus,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListHistoryCmdOptions(av[0],fUserId, dietConfig, fSessionId, fStartDateOption, fEndDateOption);


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

      cout <<"It is an admin list option " << endl;

      listOptions.setAdminListOption(true);
    }


    checkVishnuConfig(*opt);

    if ( opt->count("help")){

      helpUsage(*opt,"[options]  ");

      return 0;
    }



    /************** Call UMS connect service *******************************/


    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
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

  }// End of try bloc

  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){
    errorUsage(av[0],e.what());
    return 1;
  }

  return 0;

}// end of main


