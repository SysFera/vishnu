/**
 * \file define_transfer_identifier.cpp
 * This file defines the VISHNU command to define the file transfer identifier
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
  string newTransfertFormat;

  boost::shared_ptr<Options> opt(makeDefineIdentifierOptions(argv[0], dietConfig, newTransfertFormat));

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "format");

  //call of the api function
  DefineIdentifierFunc<TRANSFER> transferIdFunc(newTransfertFormat);
  return GenericCli().run(transferIdFunc, dietConfig, argc, argv);

}
