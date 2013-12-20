/**
 * \file PingerProxy.cpp
 * \brief This file contains the VISHNU implementation of the PingerProxy class.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */

#include <map>
#include <string>
#include "PingerProxy.hpp"
#include "DIET_client.h"
#include "utilClient.hpp"
#include <iostream>

PingerProxy::PingerProxy(){
}

PingerProxy::~PingerProxy(){
}

// Looking the service whose name starts by "service" and that is the longest
std::string
PingerProxy::getLongestService(std::vector<std::string>& list, std::string service){
  std::string res = "";
  std::vector<std::string>::iterator it;
  for (it = list.begin() ; it != list.end() ; ++it){
    if (it->find(service) == 0 &&
	it->size() > res.size()){
      res = *it;
    }
  }
  return res;
}

void
PingerProxy::selectServerToPing(std::string filterName, std::vector<boost::shared_ptr<Server> >& allServers, std::vector<boost::shared_ptr<Server> >& result){
  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = allServers.begin() ; it != allServers.end() ; ++it){
    if( (filterName.empty() ||
         it->get()->getName()==filterName) ){
      result.push_back(*it);
    }
  }
}


void
PingerProxy::ping(const std::string& server, const std::string mid, std::map<std::string, std::string>& result){
  std::string response;
// Ping the dispatcher
  communicate_dispatcher("3", response);
  if (server=="dispatcher" || (server.empty() && mid.empty()))
    result.insert(std::pair<std::string, std::string>("dispatcher", response));

// Get the list of the servers in the dispatcher
  communicate_dispatcher("2", response);
  std::vector<boost::shared_ptr<Server> > allServers;
  std::vector<boost::shared_ptr<Server> > servers;
  extractServersFromMessage(response, allServers);

  selectServerToPing(server, allServers, servers);

  // For each server ping it
  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = servers.begin() ; it != servers.end() ; ++it){
    diet_profile_t* profile = NULL;
    std::string service = getLongestService(it->get()->getServices(), "heartbeat");
    // If service asked on a machine and none corresponding
    if (!mid.empty() && service.find(mid) == std::string::npos)
      continue;

    profile = diet_profile_alloc(service,0,0,1);
    // Pinging
    if (diet_call_gen(profile, it->get()->getURI())){
      result.insert(std::pair<std::string, std::string>(it->get()->getName()+"@"+it->get()->getURI(), "FAILURE"));
    } else {
      std::string tmp;
      diet_string_get(profile, 1, tmp);
      result.insert(std::pair<std::string, std::string>(it->get()->getName()+"@"+it->get()->getURI(), tmp));
    }
    diet_profile_free(profile);
  }
}


