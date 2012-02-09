/**
 * \file delete_authentication_system.cpp
 * This file defines the VISHNU delete authentication system command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "authenticationSystemUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteAuthenticationSystemFunc {

  std::string  mauthSystemId;

  DeleteAuthenticationSystemFunc(const std::string& authSystemId ):
     mauthSystemId(authSystemId)
  {};

  int operator()(std::string sessionKey) {


     int res=deleteAuthSystem(sessionKey,mauthSystemId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::AuthSystem newAuthsystem;

  std::string authSystemId;
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




CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[option] authSystemId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[option] authSystemId");
    return 0;
  }


  DeleteAuthenticationSystemFunc apiFunc(authSystemId);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main

