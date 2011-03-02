/**
* \file OptionValueServer.hpp
* \brief This file presents the Class which manipulates VISHNU options on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef OPTION_VALUE_SERVER_H
#define OPTION_VALUE_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include "VishnuException.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "POSTGREDatabase.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;

class SessionServer;

static const std::string CLOSEPOLICY_OPT = "VISHNU_CLOSE_POLICY";
static const std::string TIMEOUT_OPT = "VISHNU_TIMEOUT";

/**
* \class OptionValueServer
* \brief OptionValueServer class implementation
*/
class OptionValueServer {

public:
  /**
  * \brief Constructor
  * \fn OptionValueServer()
  */
  OptionValueServer();
  /**
  * \brief Constructor
  * \fn OptionValueServer(UMS_Data::OptionValue*& optionvalue, SessionServer session)
  * \param optionvalue the option data structure
  * \param session The object to manipulate session
  */
  OptionValueServer(UMS_Data::OptionValue*& optionvalue, SessionServer session);
  /**
  * \brief Function to configure options on the database
  * \fn    int configureOption(bool defaultOptions = false)
  * \param defaultOptions the flag to get data from defauts options table
  * \return  raises an exception on error
  */
  int
  configureOption(bool defaultOptions = false);
  /**
  * \brief Destructor
  * \fn  ~OptionValueServer()
  */
  ~OptionValueServer();
  /**
  * \brief Function to get option data structure
  * \fn UMS_Data::OptionValue getData()
  * \return  The user data structure
  */
  UMS_Data::OptionValue*
  getData();
  /**
  * \brief Function to get option information from the database vishnu
  * \fn std::string getAttribut(std::string condition, std::string attrname="value", bool defaultOptions=false);
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \param defaultOptions the flag to get data from defauts options table
  * \return the value of the attribut or empty string if no results
  */
  std::string
  getAttribut(std::string condition, std::string attrname="value", bool defaultOptions=false);
  /**
  * \brief Function to get closure information from the database vishnu
  * \fn int getClosureInfo(std::string numuserId, std::string nameInfo = "VISHNU_CLOSE_POLICY");
  * \param numuserId The database number id of the user who wants to get closure information
  * \param nameInfo the name of the closure information
  * \return The error or success code
  */
  int
  getClosureInfo(std::string numuserId, std::string nameInfo = "VISHNU_CLOSE_POLICY");

private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The option data structure
  */
  UMS_Data::OptionValue *moptionValue;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;
  /**
  * \brief An object which encapsulates user data
  */
  SessionServer *msessionServer;

  /////////////////////////////////
  // Function
  /////////////////////////////////
  /**
  * \brief Function to check the value for a specific option
  * \fn bool isCorrectValue()
  * \return  true if the option value is correct else false
  */
  bool
  isCorrectValue();
};
#endif//OPTION_VALUE_SERVER_H
