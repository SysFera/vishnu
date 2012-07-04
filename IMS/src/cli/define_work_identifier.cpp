/**
 * \file define_user_identifier.cpp
 * This file defines the VISHNU command to define the user identifier
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"

#include "GenericCli.hpp"
#include "defineIdentifierUtils.hpp"

using namespace std;
using namespace vishnu;

int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string newAuthFormat;

  boost::shared_ptr<Options> opt(makeDefineIdentifierOptions(argv[0], dietConfig, newAuthFormat));

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "format");

  //call of the api function
  DefineIdentifierFunc<WORK>  workIdFunc(newAuthFormat);
  return GenericCli().run(workIdFunc, dietConfig, argc, argv);

}

