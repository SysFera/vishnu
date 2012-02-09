/**
 * \file update_authentication_account.cpp
 * This file defines the VISHNU update authentication account command 
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

struct UpdateAuthenticationAccountFunc {

  UMS_Data::AuthAccount mauthAccount;

  UpdateAuthenticationAccountFunc(const UMS_Data::AuthAccount& authAccount ):
     mauthAccount(authAccount)
  {};

  int operator()(std::string sessionKey) {


     int res=updateAuthAccount(sessionKey,mauthAccount);
    
     return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::AuthAccount authAccount;
  
  /******** Callback functions ******************/

  StringcallBackType fAuthSystemId( boost::bind(&UMS_Data::AuthAccount::setAuthSystemId,boost::ref(authAccount),_1));
  
  StringcallBackType fUserId( boost::bind(&UMS_Data::AuthAccount::setUserId,boost::ref(authAccount),_1));
  
  StringcallBackType fAcLogin( boost::bind(&UMS_Data::AuthAccount::setAcLogin,boost::ref(authAccount),_1));
 


  /**************** Describe options *************/

boost::shared_ptr<Options> opt= makeAuthAccountOptions(av[0],dietConfig, fAuthSystemId,fUserId,fAcLogin);

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
    helpUsage(*opt,"authSystemId ");
    return 0;
  }

  UpdateAuthenticationAccountFunc apiFunc(authAccount);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main

