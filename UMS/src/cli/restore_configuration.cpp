/**
 * \file restore_configuration.cpp
 * This file defines the VISHNU restore configuration command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
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

  string dietConfig;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);


  opt->add("filePath",
      "The path of the VISHNU configuration file",
      HIDDEN,
      filePath,
      1);

  opt->setPosition("filePath",-1);


  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] filePath");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] filePath");
    return 0;
  }

  RestoreConfigFunc restconfigFunc(filePath);
  return GenericCli().run(restconfigFunc, dietConfig, ac, av);

}// end of main


