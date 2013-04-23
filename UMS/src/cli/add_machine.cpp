/**
 * \file add_machine.cpp
 * This file defines the VISHNU add machine command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for basic_ostream, operator<<, etc
#include <string>                       // for string, operator<<

#include "GenericCli.hpp"               // for GenericCli
#include "Machine.hpp"                  // for Machine
#include "api_ums.hpp"                  // for addMachine
#include "machineUtils.hpp"             // for makeMachineOptions
#include "utilVishnu.hpp"               // for get_file_content

class Options;

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddMachineFunc {

  UMS_Data::Machine mnewMachine;

  AddMachineFunc(UMS_Data::Machine newMachine):
    mnewMachine(newMachine)
  {};

  int operator()(std::string sessionKey) {
    int res = addMachine(sessionKey,mnewMachine);
    cout << "The machine identifier is " << mnewMachine.getMachineId() << endl;
    return res;
  }
};


int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string configFile;



  /********** EMF data ************/
  UMS_Data::Machine newMachine;

  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(newMachine),_1));
  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(newMachine),_1));
  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(newMachine),_1));
  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setMachineDescription,boost::ref(newMachine),_1));

  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,configFile, fSite,fLanguage,fMachineDescription,1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddMachineFunc apiFunc(newMachine);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
