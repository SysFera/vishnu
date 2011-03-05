/**
 * \file list_histoty_cmd.cpp
 * This file defines the VISHNU list history command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "listHistoryCmdUtils.hpp"
#include "sessionUtils.hpp"
#include "utils.hpp"
#include<boost/bind.hpp>
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string dietConfig;

  string startDateOption;
  string endDateOption;
  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListCommands listCmd;

  UMS_Data::ListCmdOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListCmdOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fSessionId( boost::bind(&UMS_Data::ListCmdOptions::setSessionId,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListHistoryCmdOptions(av[0],fUserId, dietConfig, fSessionId, startDateOption, endDateOption);


  try {

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

      listHistoryCmd(sessionKey,listCmd,listOptions);


    }


    // Display the list
    if(isEmpty || (opt->count("adminListOption"))) {
      cout << listCmd << endl;
    }
    else {
      for(int i = 0; i < listCmd.getCommands().size(); i++) {
        cout << listCmd.getCommands().get(i) << endl;
      }
    }

    printSuccessMessage();
  }// End of try bloc
  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){
    errorUsage(av[0], e.what());
    return 1;
  }

  return 0;

}// end of main


