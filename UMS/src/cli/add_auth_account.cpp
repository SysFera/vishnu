/**
 * \file add_authentication_account.cpp
 * This file defines the VISHNU add authentication account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind.hpp>
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "AuthAccount.hpp"              // for AuthAccount
#include "api_ums.hpp"                  // for addAuthAccount
#include "authenticationAccountUtils.hpp"
#include "GenericCli.hpp"

class Options;

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddAuthenticationAccountFunc {
  UMS_Data::AuthAccount mnewAuthAccount;

  AddAuthenticationAccountFunc(const UMS_Data::AuthAccount& newAuthAccount):
     mnewAuthAccount(newAuthAccount)
  {};

  int operator()(std::string sessionKey) {
    int res=addAuthAccount(sessionKey,mnewAuthAccount);

    return res;
  }
};


int
main (int ac, char* av[]) {
  /******* Parsed value containers ****************/
  string configFile;

  /********** EMF data ************/
  UMS_Data::AuthAccount newAuthAccount;

  /******** Callback functions ******************/
  StringcallBackType fAuthSystemId( boost::bind(&UMS_Data::AuthAccount::setAuthSystemId,boost::ref(newAuthAccount),_1));
  StringcallBackType fUserId( boost::bind(&UMS_Data::AuthAccount::setUserId,boost::ref(newAuthAccount),_1));
  StringcallBackType fAcLogin( boost::bind(&UMS_Data::AuthAccount::setAcLogin,boost::ref(newAuthAccount),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeAuthAccountOptions(av[0],configFile, fAuthSystemId,fUserId,fAcLogin,1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddAuthenticationAccountFunc apiFunc(newAuthAccount);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
