#ifndef CONFIGURATION_PROXY_H
#define CONFIGURATION_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "UMS_Data/Configuration.h"

class ConfigurationProxy
{
private:
	UMS_Data::Configuration configuration;
	SessionProxy sessionProxy;
	SessionProxy sessionProxy;
 
public:
	void ConfigurationProxy(string filePath, SessionProxy session);
	void ConfigurationProxy(UMS_Data::Configuration config, SessionProxy session);
	int save();
	int restoreFromFile();
	int restoreFromData();
	void ~ConfigurationProxy();
	void getData();
	SessionProxy getSessionProxy();
};
#endif
