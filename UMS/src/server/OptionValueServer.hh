#ifndef OPTION_VALUE_SERVER_H
#define OPTION_VALUE_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//#include "SessionServer.hh"
#include "UserServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"

class OptionValueServer
{
private:
	//UMS_Data::OptionValue moptionValue;
	DbFactory factory;
	Database *mdatabaseVishnu;
	std::string moptionName;
	std::string moptionValue;
	//UserServer  muserServer;
 
public:
	OptionValueServer();
	OptionValueServer(std::string optionName, std::string optionValue);//, UserServer muserServer);
	int configureOption();
	int configureDefaultOption();
	~OptionValueServer(); 
	std::string getOptionValue();
	std::string getAttribut(std::string condition, std::string attrname="value", bool defaultOptions=false);
	//static UMS_Data::ListOptionsValues  list(SessionServer session, UMS_Data::ListOptOptions  options);
};
#endif
