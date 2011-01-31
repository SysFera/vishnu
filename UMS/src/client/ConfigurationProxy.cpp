#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ConfigurationProxy.h"


void ConfigurationProxy::ConfigurationProxy(string filePath, SessionProxy session)
{
}
 
void ConfigurationProxy::ConfigurationProxy(UMS_Data::Configuration config, SessionProxy session)
{
}
 
int ConfigurationProxy::save()
{
	return 0;
}
 
int ConfigurationProxy::restoreFromFile()
{
	return 0;
}
 
int ConfigurationProxy::restoreFromData()
{
	return 0;
}
 
void ConfigurationProxy::~ConfigurationProxy()
{
}
 
void ConfigurationProxy::getData()
{
}
 
SessionProxy ConfigurationProxy::getSessionProxy()
{
	return 0;
}
