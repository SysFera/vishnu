/**
 * \file VersionManager.cpp
 * \brief This file implements the VersionManager
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 20/08/12
 */

#include "VersionManager.hpp"

VersionManager::VersionManager(const UMS_Data::Version_ptr& _client,
                 const UMS_Data::Version_ptr& _server):mclient(_client), mserver(_server){
}

VersionManager::~VersionManager(){
}

bool
VersionManager::isCompatible() {

  bool compatible = true;

  if (((mclient->getMajor()) != mserver->getMajor()) ||
    (mclient->getMinor() > mserver->getMinor()) ) {
    compatible = false;
  }
  return compatible;
}

