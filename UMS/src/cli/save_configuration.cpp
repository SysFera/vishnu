/**
 * \file save_configuration.cpp
 * This file defines the VISHNU save configuration command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for basic_ostream, operator<<, etc
#include <string>                       // for string, operator<<

#include "Configuration.hpp"            // for Configuration
#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::ENV
#include "api_ums.hpp"                  // for saveConfiguration

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

  string configFile;

  /********** EMF Data ****************************/


  UMS_Data::Configuration configuration;


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  SaveConfigFunc configFunc(configuration);
  return GenericCli().run(configFunc, configFile, ac, av);

}// end of main
