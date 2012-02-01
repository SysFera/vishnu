/**
 * \file delete_authentication_account.cpp
 * This file defines the VISHNU delete authentication account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "authenticationAccountUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteAuthenticationAccountFunc {

  std::string  mauthAccountId;
  std::string  muserIdOption;

  DeleteAuthenticationAccountFunc(const std::string& authAccountId,const std::string& userIdOption ):
     mauthAccountId(authAccountId),muserIdOption (userIdOption)
  {};

  int operator()(std::string sessionKey) {


     int res=deleteAuthAccount(sessionKey,mauthAccountId,muserIdOption);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  std::string authSystemId;
  std::string userIdOption ("");

  /******** Callback functions ******************/

  boost::shared_ptr<Options> opt(new Options(av[0]));


  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("authSystemId",
      "the identifier of the user-authentication system",
      HIDDEN,
      authSystemId,
      1);

  opt->setPosition("authSystemId",1);

  opt->add("userIdOption,u",
      "an admin option which represents the VISHNU"
      "identifier of the user whose local"
      "user-authentication configuration will be deleted ",
      CONFIG,
      userIdOption);


  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"authSystemId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"authSystemId");
    return 0;
  }

  DeleteAuthenticationAccountFunc apiFunc(authSystemId,userIdOption);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main

