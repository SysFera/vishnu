#ifndef _CONFIGURATION_PROXY_H_
#define _CONFIGURATION_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.hpp"
#include "Configuration.hpp"

class ConfigurationProxy
{
 
public:
  ConfigurationProxy(const std::string& filePath, const SessionProxy& session);
  ConfigurationProxy(UMS_Data::Configuration* config, const SessionProxy& session);
  int save();
  int restoreFromFile();
  int restoreFromData();
  UMS_Data::Configuration* getData();
  SessionProxy getSessionProxy();

  ~ConfigurationProxy();

private:

  int restore(bool fromFile=true);
      
  UMS_Data::Configuration *mconfiguration;
  SessionProxy msessionProxy;
  std::string mfilePath;
};
#endif
