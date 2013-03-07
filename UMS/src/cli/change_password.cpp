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
   std::string confirmation=vishnu::takePassword("confirm new password: ");
   if (newPassword == confirmation) {
     return changePassword(muserId, oldPassword, newPassword);
   } else {
     std::cout << "Password doesn't match the confirmation" << std::endl;
     return CLI_ERROR_INVALID_PARAMETER;
   }
  }
};


int main (int ac, char* av[]){


  string userId;

  string configFile;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  opt->add("userId",
          "The VISHNU user identifier",
          HIDDEN,
          userId,
          1);
  opt->setPosition("userId", 1);

   CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt);
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if (opt->count("help")) {
    helpUsage(*opt);
    return 0;
  }

  ChgPassWordFunc chPwdFunc(userId);

  return GenericCli().runWithoutSessionKey(chPwdFunc, configFile, ac, av);
}// end of main
