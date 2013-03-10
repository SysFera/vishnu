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

  string configFile;

  /********** EMF data ************/

  UMS_Data::LocalAccount upAcLogin;
  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(upAcLogin),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,configFile,fMachineId,
                                                         fAcLogin,fSshKeyPath,fHomeDirectory);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  UpdateLocalAccountFunc apiFunc(upAcLogin);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
