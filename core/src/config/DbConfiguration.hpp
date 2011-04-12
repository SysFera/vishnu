#ifndef _DBCONFIGURATION_HPP_
#define _DBCONFIGURATION_HPP_

#include "ExecConfiguration.hpp"

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
   * \brief Get the database user name
   * \return database user name
   */
  const std::string& getDbUserName() const { return mdbUserName; }

   /**
   * \brief Get the database user password
   * \return database user password
   */
  const std::string& getDbUserPassword() const { return mdbPassword; }

protected:
  const ExecConfiguration& mexecConfig;
  db_type_t mdbType;
  std::string mdbHost;
  int mdbPort;
  std::string mdbName;
  std::string mdbUserName;
  std::string mdbPassword;
};

#endif // _DBCONFIGURATION_HPP_