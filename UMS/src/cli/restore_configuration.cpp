/**
 * \file restore_configuration.cpp
 * This file defines the VISHNU restore configuration command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct RestoreConfigFunc {

  std::string mfilePath;

  RestoreConfigFunc(std::string filePath):
    mfilePath(filePath)
  {};

  int operator()(std::string sessionKey) {
   return restoreConfiguration(sessionKey, mfilePath);
  }
};


int main (int ac, char* av[]){


  string filePath;

  string configFile;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);


  opt->add("filePath",
      "The path of the VISHNU configuration file",
      HIDDEN,
      filePath,
      1);

  opt->setPosition("filePath", 1);


  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  RestoreConfigFunc restconfigFunc(filePath);
  return GenericCli().run(restconfigFunc, configFile, ac, av);

}// end of main
