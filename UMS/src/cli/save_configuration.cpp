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
  
  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  SaveConfigFunc configFunc(configuration);
  return GenericCli().run(configFunc, dietConfig, ac, av);

}// end of main


