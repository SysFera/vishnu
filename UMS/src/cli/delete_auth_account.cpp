/**
 * \file delete_authentication_account.cpp
 * This file defines the VISHNU delete authentication account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::CONFIG, ::ENV, etc
#include "api_ums.hpp"                  // for deleteAuthAccount


using namespace std;
using namespace vishnu;

struct DeleteAuthenticationAccountFunc {

  std::string  mauthAccountId;
  std::string  muserId;

  DeleteAuthenticationAccountFunc(const std::string& authAccountId,const std::string& userId ):
     mauthAccountId(authAccountId),muserId (userId)
  {};

  int operator()(std::string sessionKey) {


     int res=deleteAuthAccount(sessionKey,mauthAccountId,muserId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string configFile;

  /********** EMF data ************/

  std::string authSystemId;
  std::string userId ("");

  /******** Callback functions ******************/

  boost::shared_ptr<Options> opt(new Options(av[0]));


  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);

  opt->add("authSystemId",
      "the identifier of the user-authentication system",
      HIDDEN,
      authSystemId,
      1);

  opt->setPosition("authSystemId",1);

  opt->add("userId,u",
      "an admin option which represents the VISHNU"
      "identifier of the user whose local"
      "user-authentication configuration will be deleted ",
      CONFIG,
      userId);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  DeleteAuthenticationAccountFunc apiFunc(authSystemId,userId);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
