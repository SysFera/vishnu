
/**
 * \file delete_user.cpp
 * This file defines the VISHNU delete_user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include<boost/bind.hpp>

#include "GenericCli.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteUserFunc {

  std::string mUserId;

  DeleteUserFunc(const std::string& userId):
    mUserId(userId)
  {};

  int operator()(std::string sessionKey) {
      
    int res = deleteUser(sessionKey,mUserId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string userId;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

  opt->setPosition("userId",-1);

  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"userId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"userId");
    return 0;
  }

  DeleteUserFunc delFunc(userId);
  return GenericCli().run(delFunc, dietConfig, ac, av); 

}// end of main




