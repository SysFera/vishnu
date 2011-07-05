/**
 * \file update_machine.cpp
 * This file defines the VISHNU update machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "machineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

#include "api_ums.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct UpDateMachineFunc {

  UMS_Data::Machine mupDateMachine;

  UpDateMachineFunc(UMS_Data::Machine upDateMachine):
  mupDateMachine(upDateMachine)
  {};

  int operator()(std::string sessionKey) {
    printSessionKeyMessage();
    int res = updateMachine(sessionKey,mupDateMachine);
    printSuccessMessage();
    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sshPublicKeyPath;

  /********** EMF data ************/

  UMS_Data::Machine upMachine;


  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(upMachine),_1));

  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(upMachine),_1));
  
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::Machine::setMachineId,boost::ref(upMachine),_1));

  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(upMachine),_1));

  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setMachineDescription,boost::ref(upMachine),_1));

  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::Machine::setStatus,boost::ref(upMachine),_1));

  // Describe options
  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage,sshPublicKeyPath,fMachineDescription);


  opt->add("machineId",
           "The identifier of the machine",
           HIDDEN,
           fMachineId,
           1);

  opt->setPosition("machineId",-1);

  opt->add("status,t",
           "The status of the machine",
           CONFIG,
           fStatus);

  CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"machineId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"machineId");
    return 0;
  }


  try{

    if(opt->count("sshPublicKeyFile")){
      // read the public key file from the public key path and set the neMachine
      upMachine.setSshPublicKey(get_file_content(sshPublicKeyPath));
    }

    UpDateMachineFunc upFunc(upMachine);
    return GenericCli().run(upFunc, dietConfig, ac, av);

  } catch(std::exception& e){

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

}// end of main


