#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "utilServer.hh"
#include "OptionValueServer.hh"
#include <exception>
#include "SystemException.hh"


OptionValueServer::OptionValueServer() {
  mdatabaseVishnu = factory.getDatabaseInstance();
}


OptionValueServer::OptionValueServer(UMS_Data::OptionValue optionvalue) {
   moptionValue = optionvalue;
   mdatabaseVishnu = factory.getDatabaseInstance();
}
 
int OptionValueServer::configureOption()
{
	return 0;
}
 
int OptionValueServer::configureDefaultOption()
{
	return 0;
}
 
OptionValueServer::~OptionValueServer()
{
}
 
UMS_Data::OptionValue OptionValueServer::getOptionValue() {
	return moptionValue;
}

//DefaultOptions est un booléen qui dit si non va chercher dans la table défault option ou pas
std::string OptionValueServer::getAttribut(std::string condition, std::string attrname, bool defaultOptions) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 
 std::string sqlCommand;
 
 if (defaultOptions) {
   sqlCommand = "SELECT "+attrname+" FROM optionu "+condition;
 }
 else {
   sqlCommand = "SELECT "+attrname+" FROM optionvalue "+condition;
 }

 std::cout <<"SQL COMMAND:"<<sqlCommand;
   
  try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
  throw e;
  }
  
  if (result->getNbTuples() != 0) {
      result->print();
      std::vector<std::string> tmp = result->get(0);  
      ii=tmp.begin();
      return (*ii);
   } 
   else {
	  return "";
	}
}

int OptionValueServer::getClosureInfo(std::string numuserId, std::string nameInfo) {

try {  
    /*To get the close policy of the user identified by the numuserId*/  
    std::string userClosepolicy = getAttribut (
		    "where users_numuserid='"+numuserId+"' and optionu_numoptionid="+
		    getAttribut("where description='"+nameInfo+"'", "numoptionid", true)
		  );
      /*If the close policy is not defined for the user*/	      
      if (userClosepolicy.size() == 0) {
	userClosepolicy = 
	getAttribut("where description='"+nameInfo+"'", "defaultvalue", true);
	return convertToInt(userClosepolicy);
      } //END If the close policy is not defined for the user*/	      
      else {
	return convertToInt(userClosepolicy);
      }
 } 
 catch (SystemException& e) {
  throw e;
 } 
  
}

/*
int OptionValueServer::getTimeout(std::string numuserId) {
  

std::string userTimeout = getAttribut (
		"where users_numuserid='"+numuserId+"' and optionu_numoptionid="+
		 getAttribut("where description='VISHNU_TIMEOUT'", "numoptionid", true)
	      );
 
  if (userTimeout.size() == 0) {
    userTimeout = 
    getAttribut("where description='VISHNU_TIMEOUT'", "defaultvalue", true);
    return convertToInt(userTimeout);
  } //END If the close policy is not defined for the user	      
  else {
    return convertToInt(userTimeout);
  } 
}
*/
 /*
UMS_Data::ListOptionsValues  OptionValueServer::list(SessionServer session, UMS_Data::ListOptOptions  options)
{
	return 0;
}
*/