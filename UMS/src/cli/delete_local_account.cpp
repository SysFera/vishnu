/**
 * \file delete_local_account.cpp
 * This file defines the VISHNU delete local account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string
#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::HIDDEN
#include "api_ums.hpp"                  // for deleteLocalAccount
#include "connectUtils.hpp"             // for makeConnectOptions

using namespace std;
using namespace vishnu;

struct DeleteLocalAccountFunc {
  string muserId;
  string mmachineId;


  DeleteLocalAccountFunc(const std::string& userId, const std::string& machineId):muserId(userId), mmachineId(machineId)
  {};

  int operator()(const std::string& sessionKey) {

    int res= deleteLocalAccount(sessionKey,muserId,mmachineId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string configFile;

  std::string userId;

  std::string machineId;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1, configFile);

  opt->setPosition("userId",1);


  opt->add("machineId",
      "the identifier of the machine associated to the local user configuration",
      HIDDEN,
      machineId,
      1);

  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  DeleteLocalAccountFunc apiFunc(userId,machineId);
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
