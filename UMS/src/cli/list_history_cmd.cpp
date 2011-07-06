/**
 * \file list_histoty_cmd.cpp
 * This file defines the VISHNU list history command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "listHistoryCmdUtils.hpp"
#include "sessionUtils.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include<boost/bind.hpp>
#include "GenericCli.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListCommandsFunc {

  UMS_Data::ListCommands mlistCmd;

  UMS_Data::ListCmdOptions mlistOptions;
  bool mfull;

  ListCommandsFunc(const UMS_Data::ListCommands& listCmd,const UMS_Data::ListCmdOptions& listOptions, bool full):
    mlistCmd(listCmd), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res = listHistoryCmd(sessionKey,mlistCmd,mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlistCmd << endl;
    }
    else {
      for(unsigned int i = 0; i <mlistCmd.getCommands().size(); i++) {
        cout << mlistCmd.getCommands().get(i) << endl;
      }
    }

    return res;
  }
};


int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string dietConfig;

  string startDateOption;
  string endDateOption;

  /********** EMF data ************/

  UMS_Data::ListCommands listCmd;

  UMS_Data::ListCmdOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListCmdOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fSessionId( boost::bind(&UMS_Data::ListCmdOptions::setSessionId,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListHistoryCmdOptions(av[0],fUserId, dietConfig, fSessionId, startDateOption, endDateOption);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  if ( opt->count("help")){
    helpUsage(*opt,"[option]");
    return 0;
  }

  /********  Process **************************/

  bool full = false;
  // Display the list
  if(isEmpty || opt->count("adminListOption")) {
    full = true;
  }

  /********  Process **************************/

    if (opt->count("adminListOption")){

      listOptions.setAdminListOption(true);
    }

    checkVishnuConfig(*opt);

    if ( opt->count("help")){

      helpUsage (*opt," [options]  ");

      return 0;
    }

    //convert the date in long format 

    if(opt->count("startDateOption")){
      listOptions.setStartDateOption(string_to_time_t(startDateOption));
    }

    if(opt->count("endDateOption")){
      listOptions.setEndDateOption(string_to_time_t(endDateOption));
    }


  ListCommandsFunc listFunc(listCmd,listOptions, full);
  return GenericCli().run(listFunc, dietConfig, ac, av);
}// end of main


  
