/**
 * \file change_password.cpp
 * This file defines the VISHNU change password command
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

struct ChgPassWordFunc {

  std::string muserId;

  ChgPassWordFunc(std::string userId):
    muserId(userId)
  {};

  int operator()() {
   std::string oldPassword=vishnu::takePassword("old password: ");
   std::string newPassword=vishnu::takePassword("new password: ");
   return changePassword(muserId,oldPassword, newPassword);
  }
};


int main (int ac, char* av[]){


  string userId;

  string dietConfig;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt->add("userId",
          "The VISHNU user identifier",
          HIDDEN,
          userId,
          1);
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
 
  ChgPassWordFunc chPwdFunc(userId);  
  return GenericCli().runWithoutSessionKey(chPwdFunc, dietConfig, ac, av);


}// end of main


