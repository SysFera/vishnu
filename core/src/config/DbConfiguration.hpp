/**
 * \file DbConfiguration.hpp
 * \brief This file contains the declaration of the DbConfiguration class
 * \author Benjamin Isnard
 * \date April
*/

#ifndef _DBCONFIGURATION_HPP_
#define _DBCONFIGURATION_HPP_

#include "ExecConfiguration.hpp"

/**
 * \class DbConfiguration
 * \brief Contains the configuration parameters for the Vishnu database
 */
class DbConfiguration {

public:

  typedef enum {
    POSTGRESQL,
    ORACLE,
    MYSQL
  } db_type_t;

  /**
   * \brief Constructor
   * \param execConfig  the configuration of the program
   */
  DbConfiguration(const ExecConfiguration& execConfig);

  /**
   * \brief Check that the configuration is correct
   */
  void check() throw (UserException);

  /**
   * \brief Get the database type
   * \return database type
   */
  db_type_t getDbType() const { return mdbType; }

  /**
   * \brief Get the database host
   * \return database host
   */
  const std::string& getDbHost() const { return mdbHost; }

  /**
   * \brief Get the database port
   * \return database port
   */
  int getDbPort() const { return mdbPort; }

  /**
   * \brief Get the database name
   * \return database name
   */
  const std::string& getDbName() const { return mdbName; }

  /**
   * \brief Get the database user name to connect to the database
   * \return database user name
   */
  const std::string& getDbUserName() const { return mdbUserName; }

   /**
   * \brief Get the database user password to connect to the database
   * \return database user password
   */
  const std::string& getDbUserPassword() const { return mdbPassword; }

protected:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
   * \brief Reference to the main program configuration
   */
  const ExecConfiguration& mexecConfig;

  /**
   * \brief Attribute type of database
   */
  db_type_t mdbType;

  /**
   * \brief Attribute host of the database
   */
  std::string mdbHost;

  /**
   * \brief Attribute port of the database
   */
  int mdbPort;

  /**
   * \brief Attribute name of the database
   */
  std::string mdbName;

  /**
   * \brief Attribute user name to connect to the database
   */
  std::string mdbUserName;

  /**
   * \brief Attribute user password to connect to the database
   */
  std::string mdbPassword;
};

#endif // _DBCONFIGURATION_HPP_
