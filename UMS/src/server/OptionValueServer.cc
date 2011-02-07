#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

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



 /*
UMS_Data::ListOptionsValues  OptionValueServer::list(SessionServer session, UMS_Data::ListOptOptions  options)
{
	return 0;
}
*/