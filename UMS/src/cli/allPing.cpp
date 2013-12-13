/**
 * \file ping.cpp
 * This file defines the VISHNU ping command
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */

#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string, operator==, etc

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::ENV, ::HIDDEN
#include "api_comm.hpp"                  // for ping
#include "cliError.hpp"                 // for CLI_ERROR_INVALID_PARAMETER
#include "utilVishnu.hpp"               // for takePassword


struct PingFunc{
  PingFunc()
  {};

  int
  operator()(){
    std::map<std::string,std::string> version;
    std::map<std::string,std::string>::iterator it;
    int res = vishnu::allPing(version);
    for (it = version.begin() ; it != version.end(); ++it)
      std::cout << "Result : " << it->first << ", " << it->second << "\n";
    return res;
  }
};



int
main (int argc, char** argv){
  std::string configFile;

  boost::shared_ptr<Options> opt(new Options(argv[0]));
  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  opt->parse_cli(argc,argv);
  opt->parse_env(env_name_mapper());
  opt->notify();

  PingFunc pings;
  return GenericCli().runWithoutSessionKey(pings, configFile, argc, argv);
}
