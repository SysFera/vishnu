/**
 * \file list_sessions.cpp
 * This file defines the VISHNU list sessions command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "utilVishnu.hpp"
#include "listHistoryCmdUtils.hpp"
#include<boost/bind.hpp>
#include "sessionUtils.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListSessionFunc {

  UMS_Data::ListSessions mlistSession;
  UMS_Data::ListSessionOptions mlistOptions;
  bool mfull;

  ListSessionFunc(UMS_Data::ListSessions listSession, UMS_Data::ListSessionOptions listOptions, bool full):
    mlistSession(listSession), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res = listSessions(sessionKey, mlistSession, mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlistSession << std::endl;
    }
    else {
      for(unsigned int i = 0; i < mlistSession.getSessions().size(); i++) {
        cout << mlistSession.getSessions().get(i) << endl;
      }
    }

   return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string startDateOption;

  std::string endDateOption;

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
           "listed (0 for INACTIVE or 1 for ACTIVE)",
           CONFIG,
           fStatus);

  opt->add("sessionClosePolicy,p",
           "Specifies the closure mode of the sessions which\n"
           "will be listed (1 for CLOSE_ON_TIMEOUT or 2 for CLOSE_ON_DISCONNECT)",
           CONFIG,
           fSessionClosePolicy);

  opt->add("sessionInactivityDelay,d",
           "specifies the inactivity delay in seconds\n"
           "of the sessions which will be listed",
           CONFIG,
           fSessionInactivityDelay);

  opt->add("machineId,m",
           "allows the user to list sessions\n"
           "opened on a specific client's machine using its name",
           CONFIG,
           fMachineId);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);


  /********  Process **************************/
  if (opt->count("adminListOption")){

    listOptions.setAdminListOption(true);
  }

  //convert the date in long format
  if(opt->count("startDateOption")){
    listOptions.setStartDateOption(string_to_time_t(startDateOption));
  }

  if(opt->count("endDateOption")){
    listOptions.setEndDateOption(string_to_time_t(endDateOption));
  }

  bool full = false;
  // Display the list
  if(isEmpty|| (opt->count("adminListOption"))) {
    full = true;
  }

  /************** Call UMS list sessions service *******************************/
  ListSessionFunc listSessFunc(listSession,listOptions, full);
  return GenericCli().run(listSessFunc, dietConfig, ac, av);

}// end of main
