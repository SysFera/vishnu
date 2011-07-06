/**
 * \file save_configuration.cpp
 * This file defines the VISHNU save configuration command 
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

struct SaveConfigFunc {

  UMS_Data::Configuration mconfiguration;

  SaveConfigFunc(UMS_Data::Configuration configuration):
    mconfiguration(configuration)
  {};

  int operator()(std::string sessionKey) {
    int res = saveConfiguration(sessionKey,mconfiguration);
    std::cout<< "The configuration path is : " << mconfiguration.getFilePath() <<std::endl;
    return res;
  }
};


int main (int ac, char* av[]){

  string dietConfig;

  /********** EMF Data ****************************/


  UMS_Data::Configuration configuration;


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);
  
  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options]");
    return 0;
  }

  SaveConfigFunc configFunc(configuration);
  return GenericCli().run(configFunc, dietConfig, ac, av);

}// end of main


