/**
 * \file delete_machine.cpp
 * This file defines the VISHNU delete_machine command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteMachineFunc {

  std::string mmachineId;

  DeleteMachineFunc(std::string machineId):
    mmachineId(machineId)
  {};

  int operator()(std::string sessionKey) {
    int res = deleteMachine(sessionKey,mmachineId);
    return res;
  }
};


int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string configFile;

  std::string machineId;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);

  opt->add("machineId",
          "The identifier of the machine",
          HIDDEN,
          machineId,1);

  opt->setPosition("machineId", 1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  DeleteMachineFunc delFunc(machineId);
  return GenericCli().run(delFunc, configFile, ac, av);

}// end of main
