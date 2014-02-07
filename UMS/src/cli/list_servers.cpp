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


struct ListServersFunc{
  std::string mserver;
  std::string mmid;
  ListServersFunc(const std::string& mid):mmid(mid)
  {};

  int
  operator()(){
    std::map<std::string,std::string> version;
    std::map<std::string,std::string>::iterator it;
    int res = vishnu::listServers(version, mmid);
    for (it = version.begin() ; it != version.end(); ++it)
      std::cout << "Server: " << it->first << ", URI: " << it->second << "\n";
    return res;
  }
};



int
main (int argc, char** argv){
  std::string configFile;
  std::string mid;

  boost::shared_ptr<Options> opt(new Options(argv[0]));
  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  opt->add("machineId,i",
           "An option for listing information about a  specific machine",
           CONFIG,
           mid);
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  ListServersFunc version(mid);
  return GenericCli().runWithoutSessionKey(version, configFile, argc, argv);
}
