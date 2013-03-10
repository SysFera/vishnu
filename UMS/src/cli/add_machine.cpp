/**
 * \file add_machine.cpp
 * This file defines the VISHNU add machine command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind.hpp>

#include "utils.hpp"
#include "GenericCli.hpp"
#include "machineUtils.hpp"
#include "cliUtil.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"

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

  string configFile;

  std::string sshPublicKeyPath;


  /********** EMF data ************/
  UMS_Data::Machine newMachine;

  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(newMachine),_1));
  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(newMachine),_1));
  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(newMachine),_1));
  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setMachineDescription,boost::ref(newMachine),_1));

  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,configFile, fSite,fLanguage,sshPublicKeyPath,fMachineDescription,1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddMachineFunc apiFunc(sshPublicKeyPath, newMachine);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
