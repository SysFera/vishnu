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
  std::string mserver;
  std::string mmid;
  PingFunc(const std::string& server, const std::string& mid):mserver(server),mmid(mid)
  {};

  int
  operator()(){
    std::map<std::string,std::string> version;
    std::map<std::string,std::string>::iterator it;
    int res = vishnu::ping(version, mserver, mmid);
    for (it = version.begin() ; it != version.end(); ++it)
      std::cout << "Result : " << it->first << ", " << it->second << "\n";
    return res;
  }
};



int
main (int argc, char** argv){
  std::string configFile;
  std::string server;
  std::string mid;

  boost::shared_ptr<Options> opt(new Options(argv[0]));
  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  opt->add("serverName,n",
          "The server to ping",
          CONFIG,
          server);

  opt->add("machineId,i",
           "An option for listing information about a  specific machine",
           CONFIG,
           mid);
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  PingFunc pings(server,mid);
  return GenericCli().runWithoutSessionKey(pings, configFile, argc, argv);
}
