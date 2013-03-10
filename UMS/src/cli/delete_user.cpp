
/**
 * \file delete_user.cpp
 * This file defines the VISHNU delete_user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include<boost/bind.hpp>

#include "GenericCli.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct DeleteUserFunc {

  std::string mUserId;

  DeleteUserFunc(const std::string& userId):
    mUserId(userId)
  {};

  int operator()(std::string sessionKey) {

    int res = deleteUser(sessionKey,mUserId);

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string configFile;

  std::string userId;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,configFile);

  opt->setPosition("userId", 1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  DeleteUserFunc delFunc(userId);
  return GenericCli().run(delFunc, configFile, ac, av);

}// end of main
