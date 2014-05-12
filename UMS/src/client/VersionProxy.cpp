/**
 * \file VersionProxy.cpp
 * \brief This file contains the VISHNU implementation of the VersionProxy class.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */

#include <map>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "VersionProxy.hpp"
#include "DIET_client.h"
#include "utilClient.hpp"
#include "UMSVishnuException.hpp"

VersionProxy::VersionProxy(){
}

VersionProxy::~VersionProxy(){
}

// Looking the service whose name starts by "service" and that is the longest
std::string
VersionProxy::getLongestService(std::vector<std::string>& list,
                                std::string service) {
  using boost::algorithm::starts_with;
  std::string res;

  BOOST_FOREACH(const std::string& item, list) {
    if (starts_with(item, service) &&
        item.length() > res.length()) {
      res = item;
    }
  }

  return res;
}

void
VersionProxy::selectServerToPing(std::string filterName,
                                 std::vector<boost::shared_ptr<Server> >& allServers,
                                 std::vector<boost::shared_ptr<Server> >& result) {
  std::vector<boost::shared_ptr<Server> >::iterator it;

  for (it = allServers.begin() ; it != allServers.end() ; ++it) {
    if (filterName.empty() || (*it)->getName() == filterName ) {
      result.push_back(*it);
    }
  }
}


void
VersionProxy::getVersion(const std::string& server,
                         const std::string mid,
                         std::map<std::string, std::string>& result) {
  std::string response;
  bool empty = true;
  int res = communicate_dispatcher("3", response, true);

  if (server == "dispatcher" ||
      (server.empty() && mid.empty() && res !=- 1)) {
    result.insert(std::make_pair("dispatcher", response));
    empty = false;
  }
  response = "";

  // Get the list of the servers in the dispatcher
  communicate_dispatcher("2", response, true);
  std::vector<boost::shared_ptr<Server> > allServers;
  std::vector<boost::shared_ptr<Server> > servers;
  extractServersFromMessage(response, allServers);
  getServersListFromConfig(allServers);
  selectServerToPing(server, allServers, servers);

  // For each server get its version
  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = servers.begin() ; it != servers.end() ; ++it) {
    std::string service = getLongestService((*it)->getServices(), "heartbeat");
    // If service asked on a machine and none corresponding
    if (!mid.empty() && service.find(mid) == std::string::npos)
      continue;

    diet_profile_t* profile = diet_profile_alloc(service, 0);
    // Getting version
    if (abstract_call_gen(profile, it->get()->getURI(), true)){
      result.insert(std::pair<std::string, std::string>(it->get()->getName()+"@"+it->get()->getURI(), "Server unavailable"));
    } else {
      std::string versionInfo;
      diet_string_get(profile, 1, versionInfo);
      result.insert(std::pair<std::string, std::string>(it->get()->getName()+"@"+it->get()->getURI(), versionInfo));
    }
    diet_profile_free(profile);
    empty = false;
  }
  if(empty){
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "No corresponding server found");
  }
}

void
VersionProxy::list(const std::string mid, std::map<std::string, std::string>& result){
  std::string response;
  // Get version of the dispatcher
  bool empty = true;


  // Get the list of the servers in the dispatcher
  communicate_dispatcher("2", response, true);
  std::vector<boost::shared_ptr<Server> > servers;
  extractServersFromMessage(response, servers);
  getServersListFromConfig(servers);

  // For each server get its version
  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = servers.begin() ; it != servers.end() ; ++it){
    std::string service = getLongestService(it->get()->getServices(), "heartbeat");
    // If service asked on a machine and none corresponding
    if (!mid.empty() && service.find(mid) == std::string::npos)
      continue;
    result.insert(std::pair<std::string, std::string>(it->get()->getName(), it->get()->getURI()));
    empty = false;
  }
  if(empty){
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "No corresponding server found");
  }
}
