/**
 * \file list_machines.cpp
 * This file defines the VISHNU list machine command
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
#include "ListMachineOptions.hpp"       // for ListMachineOptions
#include "ListMachines.hpp"             // for ListMachines
#include "Options.hpp"                  // for Options, ::CONFIG
#include "api_ums.hpp"                  // for listMachines
#include "ecorecpp/mapping/EList.hpp"   // for EList
#include "listMachineUtils.hpp"         // for makeListMachineOptions
#include "utils.hpp"                    // for operator<<


struct ListMachineFunc {

  UMS_Data::ListMachines mlsMachine;
  UMS_Data::ListMachineOptions mlistOptions;
  bool mfull;

  ListMachineFunc(UMS_Data::ListMachines lsMachine, UMS_Data::ListMachineOptions listOptions, bool full):
    mlsMachine(lsMachine), mlistOptions(listOptions), mfull(full)
  {}

  int operator()(std::string sessionKey) {
    int res = vishnu::listMachines(sessionKey,mlsMachine,mlistOptions);
    // Display the list
    if(mfull) {
      std::cout << mlsMachine << std::endl;
    } else {
      for(unsigned int i = 0; i < mlsMachine.getMachines().size(); i++) {
        std::cout << mlsMachine.getMachines().get(i) << std::endl;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]) {
  /******* Parsed value containers ****************/
  std::string configFile;

  /********** EMF data ************/
  UMS_Data::ListMachines lsMachine;

  UMS_Data::ListMachineOptions listOptions;

  /******** Callback functions ******************/
  boost::function1<void,std::string> fUserId( boost::bind(&UMS_Data::ListMachineOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,std::string> fMachineId( boost::bind(&UMS_Data::ListMachineOptions::setMachineId,boost::ref(listOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],fUserId, configFile, fMachineId);


  opt->add("listAllmachine,a",
           "An option for listing all VISHNU machines",
           CONFIG);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  /********  Process **************************/
  if (opt->count("listAllmachine")){

    listOptions.setListAllMachine(true);
  }

  bool full = false;
  // Display the list
  if(isEmpty || opt->count("listAllmachine")) {
    full = true;
  }

  ListMachineFunc listFunc(lsMachine,listOptions, full);
  return GenericCli().run(listFunc, configFile, ac, av);
}// end of main
