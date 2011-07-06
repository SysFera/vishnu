/**
 * \file delete_local_account.cpp
 * This file defines the VISHNU delete local account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "connectUtils.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteLocalAccountFunc {
  string muserId;
  string mmachineId;


  DeleteLocalAccountFunc(const std::string& userId, const std::string& machineId):muserId(userId), mmachineId(machineId)
  {};

  int operator()(const std::string& sessionKey) {

    int res= deleteLocalAccount(sessionKey,muserId,mmachineId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string userId;

  std::string machineId;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1, dietConfig);

  opt->setPosition("userId",1);


  opt->add("machineId",
      "the identifier of the machine associated to the local user configuration",
      HIDDEN,
      machineId,
      1);

  opt->setPosition("machineId",1);

  CLICmd cmd = CLICmd (ac, av, opt);
 
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"userId machineId ");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);

  if ( opt->count("help")){
    helpUsage(*opt,"userId machineId ");
    return 0;
  }

  DeleteLocalAccountFunc apiFunc(userId,machineId);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main




