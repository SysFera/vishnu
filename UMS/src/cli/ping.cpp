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
    std::string version;
    int res = vishnu::ping(mserver, version, mmid);
    std::cout << "Result : " << version << "\n";
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

  opt->add("serverName",
          "The server to ping",
          HIDDEN,
          server,
          1);
  opt->setPosition("serverName", 1);

  opt->add("machineId,i",
           "An option for listing information about a  specific machine",
           CONFIG,
           mid);
  opt->parse_cli(argc,argv);
  opt->parse_env(env_name_mapper());
  opt->notify();

  PingFunc pings(server,mid);
  return GenericCli().runWithoutSessionKey(pings, configFile, argc, argv);
}
