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

  if ((mclient->getMajor()) != mserver->getMajor()) {
    compatible = false;
    merrormsg = "Compatibility problem: the major of the server version is different of the client's major version";
  }

  if ((mclient->getMinor()) > mserver->getMinor()) {
    compatible = false;
    merrormsg = "Compatibility problem: The minor of the server version (minor="
                +vishnu::convertToString<>(mserver->getMinor())+") is lower than the client's minor version";
  }

  return compatible;
}

std::string
VersionManager::getError() {
  return merrormsg;
}