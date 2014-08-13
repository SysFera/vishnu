/**
 * \file update_machine.cpp
 * This file defines the VISHNU update machine command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <sstream>                      // for basic_stringbuf<>::int_type, etc
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "Machine.hpp"                  // for Machine
#include "Options.hpp"                  // for Options, ::CONFIG, ::HIDDEN
#include "api_ums.hpp"                  // for updateMachine
#include "cliError.hpp"                 // for CLI_ERROR_RUNTIME
#include "cliUtil.hpp"                  // for errorUsage
#include "machineUtils.hpp"             // for makeMachineOptions
#include "utilVishnu.hpp"               // for get_file_content

using namespace std;
using namespace vishnu;

struct UpDateMachineFunc {

  UMS_Data::Machine mupDateMachine;

  UpDateMachineFunc(UMS_Data::Machine upDateMachine):
    mupDateMachine(upDateMachine)
  {};

  int operator()(std::string sessionKey) {
    int res = updateMachine(sessionKey,mupDateMachine);
    return res;
  }
};


int main (int ac, char* av[]){


  string configFile;
  UMS_Data::Machine upMachine;

  boost::function1<void,string> fHostAddress( boost::bind(&UMS_Data::Machine::setAddress,boost::ref(upMachine),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::Machine::setMachineId,boost::ref(upMachine),_1));
  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setDescription,boost::ref(upMachine),_1));
  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::Machine::setStatus,boost::ref(upMachine),_1));

  // Describe options
  boost::shared_ptr<Options> opt= makeMachineOptions(av[0],
      configFile,
      fMachineId,
      fHostAddress,
      fMachineDescription,
      0);


  opt->add("machineId",
           "The machine identifier",
           HIDDEN,
           fMachineId,
           1);

  opt->setPosition("machineId", 1);

  opt->add("status,t",
           "The status of the machine",
           CONFIG,
           fStatus);

  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, ac, av);


  try {
    UpDateMachineFunc upFunc(upMachine);
    return GenericCli().run(upFunc, configFile, ac, av);

  } catch(std::exception& e) {
    errorUsage(av[0],e.what());
    return CLI_ERROR_RUNTIME;
  }
}// end of main
