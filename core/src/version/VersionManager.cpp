/**
 * \file VersionManager.cpp
 * \brief This file implements the VersionManager
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 20/08/12
 */

#include "VersionManager.hpp"
#include "utilVishnu.hpp"

VersionManager::VersionManager(const UMS_Data::Version_ptr& _client,
                 const UMS_Data::Version_ptr& _server):mclient(_client), mserver(_server){
}

VersionManager::~VersionManager(){
}

bool
VersionManager::isCompatible() {

  bool compatible = true;

  merrormsg = "";

  if ((mclient->getMajor()) != mserver->getMajor()) {
    compatible = false;
    merrormsg = "Compatibility problem: the major of the server version (v"+
                mserver->getStringformat()+") is different of the client's major version (v"
                +mclient->getStringformat()+")";
  }

  //If no messages are already registered
  if (merrormsg.size() == 0) {
    if ((mclient->getMinor()) > mserver->getMinor()) {
      compatible = false;
      merrormsg = "Compatibility problem: The minor of the server version (v"
                  +mserver->getStringformat()+") is lower than the client's minor version"
                  " (v"+mclient->getStringformat()+")";
    }
  }
  return compatible;
}

std::string
VersionManager::getError() {
  return merrormsg;
}
