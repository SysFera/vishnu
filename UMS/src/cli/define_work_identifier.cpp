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
#include "sessionUtils.hpp"

#include "GenericCli.hpp"
#include "defineIdentifierUtils.hpp"

using namespace std;
using namespace vishnu;

int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string configFile;
  string newAuthFormat;

  boost::shared_ptr<Options> opt(makeDefineIdentifierOptions(argv[0], configFile, newAuthFormat));

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  DefineIdentifierFunc<WORK>  workIdFunc(newAuthFormat);
  return GenericCli().run(workIdFunc, configFile, argc, argv);

}
