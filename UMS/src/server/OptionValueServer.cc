/**
* \file OptionValueServer.cpp
* \brief This file implements the Class which manipulates VISHNU options on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/
#include "OptionValueServer.hh"

/**
* \brief Constructor
* \fn OptionValueServer() 
*/
OptionValueServer::OptionValueServer() {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}
/**
* \brief Constructor
* \fn OptionValueServer(UMS_Data::OptionValue optionvalue)
* \param optionvalue the option data structure 
*/
OptionValueServer::OptionValueServer(UMS_Data::OptionValue optionvalue):moptionValue(optionvalue) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}
/**
* \brief Function to configure options on the database
* \fn    int configureOption()
* \return  raises an exception on error
*/
int 
OptionValueServer::configureOption() {
  return 0;
}
/**
* \brief Function to configure default options on the database
* \fn    int configureDefaultOption()
* \return  raises an exception on error
*/  
int 
OptionValueServer::configureDefaultOption() {
  return 0;
}
/**
* \brief Destructor
* \fn  ~OptionValueServer()
*/
OptionValueServer::~OptionValueServer() {
}
/**
* \brief Function to get option data structure
* \fn UMS_Data::OptionValue getData()
* \return  The user data structure
*/ 
UMS_Data::OptionValue 
OptionValueServer::getData() {
  return moptionValue;
}
/**
* \brief Function to get option information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname, defaultOptions);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \param defaultOptions the flag to get data from defauts options table
* \return the value of the attribut or empty string if no results
*/
std::string 
OptionValueServer::getAttribut(std::string condition, std::string attrname, bool defaultOptions) {

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
/**
* \brief Function to get closure information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname, defaultOptions);
* \param numuserId The database number id of the user who wants to get closure information
* \param nameInfo the name of the closure information 
*/
int 
OptionValueServer::getClosureInfo(std::string numuserId, std::string nameInfo) {
  
  std::string userCloseInfo;

  try {  
    //To get the closure information of the user identified by the numuserId 
    userCloseInfo = getAttribut ("where users_numuserid='"+numuserId+"' and optionu_numoptionid="+
    getAttribut("where description='"+nameInfo+"'", "numoptionid", true));

    //If the closure information is not defined for the user	      
    if (userCloseInfo.size() == 0) {
      //To get the closure information from the table with the default options values
      userCloseInfo = getAttribut("where description='"+nameInfo+"'", "defaultvalue", true);
      return convertToInt(userCloseInfo);
    } //END If the closure information is not defined for the user	          
    else {
      return convertToInt(userCloseInfo);
    }
  } 
  catch (SystemException& e) {
  throw e;
  }   
}
 /*
UMS_Data::ListOptionsValues  OptionValueServer::list(SessionServer session, UMS_Data::ListOptOptions  options)
{
	return 0;
}
*/