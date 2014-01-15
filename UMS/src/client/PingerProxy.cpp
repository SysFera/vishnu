/**
 * \file PingerProxy.cpp
 * \brief This file contains the VISHNU implementation of the PingerProxy class.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */

#include <map>
#include <string>
#include "PingerProxy.hpp"
#include "VersionProxy.hpp"
#include "DIET_client.h"
#include "utilClient.hpp"
#include <iostream>

PingerProxy::PingerProxy(){
}

PingerProxy::~PingerProxy(){
}

void
PingerProxy::ping(const std::string& server, const std::string mid, std::map<std::string, std::string>& result){
  getVersion(server, mid, result);
}


