/**
 * \file DbConfiguration.cpp
 * \brief This file contains the definitions of the DbConfiguration class
 * \author Benjamin Isnard
 * \date April
*/

#include "DbConfiguration.hpp"

using namespace std;

const int DbConfiguration::defaultDbPoolSize = 10;  //%RELAX<MISRA_0_1_3> Used in this file

/**
 * \brief Constructor
 * \param execConfig  the configuration of the program
 */
DbConfiguration::DbConfiguration(const ExecConfiguration& execConfig) : mexecConfig(execConfig),
                                                                        mdbType(POSTGRESQL), mdbPort(0),
                                                                        mdbPoolSize(defaultDbPoolSize)
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
  mexecConfig.getConfigValue<unsigned>(vishnu::DBPORT, mdbPort);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBNAME, mdbName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBUSERNAME, mdbUserName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBPASSWORD, mdbPassword);
  mexecConfig.getConfigValue<unsigned>(vishnu::DBPOOLSIZE, mdbPoolSize);

}
