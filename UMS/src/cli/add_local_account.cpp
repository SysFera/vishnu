/**
 * \file add_local_account.cpp
 * This file defines the VISHNU add local account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <stdlib.h>                     // for getenv
#include <boost/bind.hpp>           // for arg
#include <boost/filesystem/operations.hpp>  // for create_directories, etc
#include <boost/filesystem/path.hpp>    // for path
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for basic_ostream, ofstream, etc
#include <string>                       // for string, operator+, etc
#include "LocalAccount.hpp"             // for LocalAccount
#include "api_ums.hpp"                  // for addLocalAccount
#include "ecore_forward.hpp"            // for EString
#include "localAccountUtils.hpp"
#include "GenericCli.hpp"

class Options;

namespace po = boost::program_options;
struct AddLocalAccountFunc {

  UMS_Data::LocalAccount mnewAcLogin;

  AddLocalAccountFunc(UMS_Data::LocalAccount newAcLogin): mnewAcLogin(newAcLogin)
  {}

  int operator()(std::string sessionKey) {
    std::string sshPublicKey;
    return vishnu::addLocalAccount(sessionKey, mnewAcLogin, sshPublicKey);
  }
};

int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  std::string configFile;

  /********** EMF data ************/

  UMS_Data::LocalAccount localAccountInfo;

  /******** Callback functions ******************/

  boost::function1<void,std::string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(localAccountInfo),_1));
  boost::function1<void,std::string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(localAccountInfo),_1));
  boost::function1<void,std::string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(localAccountInfo),_1));
  boost::function1<void,std::string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(localAccountInfo),_1));


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt = makeLocalAccountOptions(av[0],
      fUserId,
      configFile,
      fMachineId,
      fAcLogin,
      fHomeDirectory,
      0);

  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddLocalAccountFunc apiFunc(localAccountInfo);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
