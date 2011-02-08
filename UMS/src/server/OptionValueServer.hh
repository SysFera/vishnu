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
	UMS_Data::OptionValue moptionValue;
	DbFactory factory;
	Database *mdatabaseVishnu;
	/*std::string moptionName;
	std::string moptionValue;
	UserServer  muserServer;*/
 
public:
	OptionValueServer();
	OptionValueServer(UMS_Data::OptionValue optionvalue);
	int configureOption();
	int configureDefaultOption();
	~OptionValueServer(); 
	UMS_Data::OptionValue getOptionValue();
	std::string getAttribut(std::string condition, std::string attrname="value", bool defaultOptions=false);
	int getClosureInfo(std::string numuserId, std::string nameInfo = "VISHNU_CLOSE_POLICY");
	//int getTimeout(std::string numuserId);
	//static UMS_Data::ListOptionsValues  list(SessionServer session, UMS_Data::ListOptOptions  options);
};
#endif
