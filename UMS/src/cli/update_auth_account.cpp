/**
 * \file update_authentication_account.cpp
 * This file defines the VISHNU update authentication account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "AuthAccount.hpp"              // for AuthAccount
#include "GenericCli.hpp"               // for GenericCli
#include "api_ums.hpp"                  // for updateAuthAccount
#include "authenticationAccountUtils.hpp"  // for StringcallBackType, etc

class Options;


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

  string configFile;

  /********** EMF data ************/

  UMS_Data::AuthAccount authAccount;

  /******** Callback functions ******************/

  StringcallBackType fAuthSystemId( boost::bind(&UMS_Data::AuthAccount::setAuthSystemId,boost::ref(authAccount),_1));

  StringcallBackType fUserId( boost::bind(&UMS_Data::AuthAccount::setUserId,boost::ref(authAccount),_1));

  StringcallBackType fAcLogin( boost::bind(&UMS_Data::AuthAccount::setAcLogin,boost::ref(authAccount),_1));



  /**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeAuthAccountOptions(av[0],configFile, fAuthSystemId,fUserId,fAcLogin);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  UpdateAuthenticationAccountFunc apiFunc(authAccount);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
