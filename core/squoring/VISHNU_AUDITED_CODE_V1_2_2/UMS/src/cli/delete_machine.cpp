/**
 * \file delete_machine.cpp
 * This file defines the VISHNU delete_machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"

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

  string dietConfig;

  std::string machineId;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("machineId",
          "The identifier of the machine",
          HIDDEN,
          machineId,1);

  opt->setPosition("machineId",-1);

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

  DeleteMachineFunc delFunc(machineId);
  return GenericCli().run(delFunc, dietConfig, ac, av); 

}// end of main


