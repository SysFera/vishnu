/**
 * \file reset_password.cpp
 * This file defines the VISHNU reset password command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options
#include "api_ums.hpp"                  // for resetPassword
#include "connectUtils.hpp"             // for makeConnectOptions

using namespace std;
using namespace vishnu;

struct ResetPassWordFunc {

  std::string muserId;
  std::string mtmpPassword;

  ResetPassWordFunc(std::string userId, std::string tmpPassword):
    muserId(userId),mtmpPassword(tmpPassword)
  {};

  int operator()(std::string sessionKey) {
   int res = resetPassword(sessionKey,muserId, mtmpPassword);
   return res;
  }
};

int main (int ac, char* av[]){
  string userId;
  string configFile;


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,configFile);

  opt->setPosition("userId", 1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  std::string tmpPassword;
  ResetPassWordFunc resetPassWordFunc(userId, tmpPassword);
  return GenericCli().run(resetPassWordFunc, configFile, ac, av);
}  // end of main
