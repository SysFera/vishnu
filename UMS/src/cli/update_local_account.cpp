/**
 * \file update_local_account.cpp
 * This file defines the VISHNU update local account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "LocalAccount.hpp"             // for LocalAccount
#include "api_ums.hpp"                  // for updateLocalAccount
#include "localAccountUtils.hpp"        // for makeLocalAccountOptions

class Options;

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
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(upAcLogin),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,configFile,fMachineId,
                                                         fAcLogin,fHomeDirectory);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  UpdateLocalAccountFunc apiFunc(upAcLogin);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
