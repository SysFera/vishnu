/**
 * \file add_machine.cpp
 * This file defines the VISHNU add machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "utils.hpp"
#include "cliUtil.hpp"
#include "machineUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddMachineFunc {

  std::string msshPublicKeyPath;
  UMS_Data::Machine mnewMachine;

  AddMachineFunc(std::string sshPublicKeyPath, UMS_Data::Machine newMachine):
    msshPublicKeyPath(sshPublicKeyPath), mnewMachine(newMachine)
  {};

  int operator()(std::string sessionKey) {
    mnewMachine.setSshPublicKey(get_file_content(msshPublicKeyPath));
    int res = addMachine(sessionKey,mnewMachine);
    cout << "The machine identifier is " << mnewMachine.getMachineId() << endl;
    return res;
  }
};


int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sshPublicKeyPath;


  /********** EMF data ************/
  UMS_Data::Machine newMachine;

  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(newMachine),_1));
  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(newMachine),_1));
  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(newMachine),_1));
  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setMachineDescription,boost::ref(newMachine),_1));

  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage,sshPublicKeyPath,fMachineDescription,1);

  CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"name site language sshPublicKeyFile machineDescription");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"name site language sshPublicKeyFile machineDescription");
    return 0;
  }
 
  AddMachineFunc apiFunc(sshPublicKeyPath, newMachine);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main


