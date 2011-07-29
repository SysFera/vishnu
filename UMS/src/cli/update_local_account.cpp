/**
 * \file update_local_account.cpp
 * This file defines the VISHNU update local account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "localAccountUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct UpdateLocalAccountFunc {

  UMS_Data::LocalAccount mupAcLogin;
  
  UpdateLocalAccountFunc(UMS_Data::LocalAccount upAcLogin): mupAcLogin(upAcLogin)
  {};

  int operator()(std::string sessionKey) {

    int res= updateLocalAccount(sessionKey,mupAcLogin);

    return res;


  }
};
int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::LocalAccount upAcLogin;
  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(upAcLogin),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,dietConfig,fMachineId,
                                                         fAcLogin,fSshKeyPath,fHomeDirectory);

  CLICmd cmd = CLICmd (ac, av, opt);
  
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"userId machineId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"userId machineId");
    return 0;
  }
 
  UpdateLocalAccountFunc apiFunc(upAcLogin);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main
