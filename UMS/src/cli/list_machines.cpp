/**
 * \file list_machines.cpp
 * This file defines the VISHNU list machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
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

  UMS_Data::ListMachines lsMachine;

  UMS_Data::ListMachineOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListMachineOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListMachineOptions::setMachineId,boost::ref(listOptions),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],fUserId, dietConfig, fMachineId);


  opt->add("listAllmachine,a",
           "An option for listing all VISHNU machines",
           CONFIG);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    bool isEmpty=opt->empty();//if no value was given in the command line

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/

    if (opt->count("listAllmachine")){

      listOptions.setListAllmachine(true);
    }


    checkVishnuConfig(*opt);

    if ( opt->count("help")){

      helpUsage(*opt," [options]");

      return 0;
    }

    /************** Call UMS connect service *******************************/


    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
    
      errorUsage(av[0],dietErrorMsg,EXECERROR);

           return  CLI_ERROR_DIET ;

    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      listMachine(sessionKey,lsMachine,listOptions);


    }


    // Display the list
    if(isEmpty || opt->count("listAllmachine")) {
      cout << lsMachine << endl;
    }
    else {
      for(int i = 0; i < lsMachine.getMachines().size(); i++) {
        cout << lsMachine.getMachines().get(i) << endl;
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

  catch(std::exception& e){ // catch all std runtime error

    errorUsage(av[0], e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


