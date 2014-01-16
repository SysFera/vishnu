/** \file api_comm.cpp
 * \brief This file contains the VISHNU api functions for the core package.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */

#include <map>
#include <string>
#include "api_comm.hpp"
#include "PingerProxy.hpp"
#include "VersionProxy.hpp"

int
vishnu::ping(std::map<std::string, std::string>& result,
             const std::string& server,
             const std::string& mid){
  PingerProxy proxy;
  proxy.ping(server, mid, result);
  return 0;
}

int
vishnu::getServerVersion(std::map<std::string, std::string>& result,
                         const std::string& server,
                         const std::string& mid){
  VersionProxy proxy;
  proxy.getVersion(server, mid, result);
  return 0;
}


int
vishnu::listServers(std::map<std::string, std::string>& result,
                    const std::string& mid){
  VersionProxy proxy;
  proxy.list(mid, result);
}

