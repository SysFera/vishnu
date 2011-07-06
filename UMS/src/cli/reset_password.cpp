/**
 * \file reset_password.cpp
 * This file defines the VISHNU reset password command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ResetPassWordFunc {

  std::string muserId;
  std::string mtmpPassword;

  ResetPassWordFunc(std::string userId, std::string tmpPassword):
    muserId(userId),mtmpPassword(tmpPassword)
  {};

  int operator()(std::string sessionKey) {
   return resetPassword(sessionKey,muserId, mtmpPassword);
  }
};

int main (int ac, char* av[]){


  string userId;

  string dietConfig;


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

  opt->setPosition("userId",-1);

  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] userId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] userId");
    return 0;
  }
  std::string tmpPassword;
  ResetPassWordFunc resetPassWordFunc(userId, tmpPassword);
  return GenericCli().run(resetPassWordFunc, dietConfig, ac, av);

}// end of main


