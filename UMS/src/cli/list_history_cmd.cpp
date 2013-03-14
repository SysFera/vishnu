/**
 * \file list_histoty_cmd.cpp
 * This file defines the VISHNU list history command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for ostream, cout, endl
#include <string>                       // for string
#include "GenericCli.hpp"               // for GenericCli
#include "ListCmdOptions.hpp"           // for ListCmdOptions
#include "ListCommands.hpp"             // for ListCommands
#include "Options.hpp"                  // for Options
#include "api_ums.hpp"                  // for listHistoryCmd
#include "ecorecpp/mapping/EList.hpp"   // for EList
#include "listHistoryCmdUtils.hpp"      // for makeListHistoryCmdOptions
#include "utilVishnu.hpp"               // for string_to_time_t
#include "utils.hpp"                    // for operator<<

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

  string configFile;

  string startDateOption;
  string endDateOption;

  /********** EMF data ************/

  UMS_Data::ListCommands listCmd;

  UMS_Data::ListCmdOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListCmdOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fSessionId( boost::bind(&UMS_Data::ListCmdOptions::setSessionId,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListHistoryCmdOptions(av[0],fUserId, configFile, fSessionId, startDateOption, endDateOption);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);


  /********  Process **************************/

  bool full = false;
  // Display the list
  if(isEmpty || opt->count("adminListOption")) {
    full = true;
  }

  /********  Process **************************/

    if (opt->count("adminListOption")) {
      listOptions.setAdminListOption(true);
    }


    //convert the date in long format

    if(opt->count("startDateOption")){
      listOptions.setStartDateOption(string_to_time_t(startDateOption));
    }

    if(opt->count("endDateOption")){
      listOptions.setEndDateOption(string_to_time_t(endDateOption));
    }


  ListCommandsFunc listFunc(listCmd,listOptions, full);
  return GenericCli().run(listFunc, configFile, ac, av);
}// end of main
