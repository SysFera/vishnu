/**
 * \file DbConfiguration.cpp
 * \brief This file contains the definitions of the DbConfiguration class
 * \author Benjamin Isnard
 * \date April
*/

#include "DbConfiguration.hpp"

using namespace std;

/**
 * \brief Constructor
 * \param execConfig  the configuration of the program
 */
DbConfiguration::DbConfiguration(const ExecConfiguration& execConfig) : mexecConfig(execConfig)
{
}

/**
 * \brief Check that the configuration is correct
 */
void DbConfiguration::check() throw (UserException)
{
  string dbTypeStr;
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBTYPE, dbTypeStr);
  if (dbTypeStr == "oracle") {
    mdbType = DbConfiguration::ORACLE;
  } else if (dbTypeStr == "postgresql") {
    mdbType = DbConfiguration::POSTGRESQL;
  } else if (dbTypeStr == "mysql") {
    mdbType = DbConfiguration::MYSQL;
  } else {
    throw UserException(ERRCODE_INVALID_PARAM, "Configuration for database type is invalid (must be 'oracle' or 'postgresql' or 'mysql')");
  }
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBHOST, mdbHost);
  mexecConfig.getConfigValue<int>(vishnu::DBPORT, mdbPort);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBNAME, mdbName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBUSERNAME, mdbUserName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBPASSWORD, mdbPassword);
}

