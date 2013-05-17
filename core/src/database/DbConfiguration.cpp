/**
 * \file DbConfiguration.cpp
 * \brief This file contains the definitions of the DbConfiguration class
 * \author Benjamin Isnard
 * \date April
*/

#include "DbConfiguration.hpp"
#include <iostream>
#include <boost/format.hpp>
#include <cstdlib>

using namespace std;

const unsigned DbConfiguration::defaultDbPoolSize = 10;  //%RELAX<MISRA_0_1_3> Used in this file

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

  // Common params
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBHOST, mdbHost);
  mexecConfig.getConfigValue<unsigned>(vishnu::DBPORT, mdbPort);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBNAME, mdbName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBUSERNAME, mdbUserName);
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::DBPASSWORD, mdbPassword);
  mexecConfig.getConfigValue<unsigned>(vishnu::DBPOOLSIZE, mdbPoolSize);

  // SSL params
  bool ret = mexecConfig.getConfigValue<bool>(vishnu::DB_USE_SSL, museSsl);
  if (ret && museSsl) {
    bool caset = mexecConfig.getConfigValue<std::string>(vishnu::DB_SSL_CA_FILE, msslCaFile);
    if (!caset && (dbTypeStr == "postgresql")){
      msslCaFile = std::string(getenv("HOME"))+"/.postgresql/root.crt";
    }
    // For logging
    std::cerr << boost::format("[VISHNU][INFO] Expecting ciphered database connections...\n"
                               "  > CA: %1%\n")%msslCaFile;
  }
}
