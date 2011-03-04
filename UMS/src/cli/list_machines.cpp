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

      cout <<"We nedd to list all VISHNU machines " << endl;

      listOptions.setListAllmachine(true);
    }


    if(opt->count("sessionKey")){

      cout <<"the session key is : " << sessionKey << endl;
    }


    checkVishnuConfig(*opt);

    if ( opt->count("help")){

      helpUsage(*opt," [options]");

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


