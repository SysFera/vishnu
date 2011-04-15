/**
 * \file list_local_accounts.cpp
 * This file defines the VISHNU list local account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "listMachineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListLocalAccounts lsLocalAccount;

  UMS_Data::ListLocalAccOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListLocalAccOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListLocalAccOptions::setMachineId,boost::ref(listOptions),_1));

  /**************** Describe options *************/


  boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],fUserId, dietConfig, fMachineId);


  opt->add("adminListOption,a",
           "is an admin option for listing all local configurations of all users	",
           CONFIG);


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

      helpUsage(*opt," [options]");

      return 0;
    }


    /************** Call UMS list loacl account service *******************************/


    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
     
      errorUsage(av[0],dietErrorMsg,EXECERROR);

           return  CLI_ERROR_DIET ;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      listLocalAccount(sessionKey,lsLocalAccount,listOptions);

    }


    // Display the list
    if(isEmpty|| (opt->count("adminListOption"))) {
      cout << lsLocalAccount << endl;
    }
    else {
      for(unsigned int i = 0; i < lsLocalAccount.getAccounts().size(); i++) {
        cout << lsLocalAccount.getAccounts().get(i) ;
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
 
  //check the bad session key

    if (checkBadSessionKeyError(e)){

      removeBadSessionKeyFromFile(getppid());
    }
 

    return e.getMsgI() ;

  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0], e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


