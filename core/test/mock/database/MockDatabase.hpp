/**
 * \file MockDatabase.hpp
 * \brief This file presents a SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _MOCKDATABASE_H_
#define _MOCKDATABASE_H_

#include "Database.hpp"

/**
 * \class MockDatabase
 * \brief Mock implementation of the Database
 */
class MockDatabase : public Database{
public :
  /**
   * \brief Function to process the request in the database
   * \param request The request to process (must contain a SINGLE SQL statement without a semicolumn)
   * \param transacId the id of the transaction if one is used
   * \return 0 on success, an error code otherwise
   */
  int
  process(std::string request, int transacId = -1);
  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  int
  connect();
  /**
   * \brief Constructor, raises an exception on error
   * \fn MockDatabase(DbConfiguration dbConfig)
   * \param dbConfig  the configuration of the database client
   */
  MockDatabase(DbConfiguration dbConfig);
  /**
   * \fn ~MockDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~MockDatabase();
  /**
  * \brief To get the result of a select request
  * \param request The request to process
  * \param transacId the id of the transaction if one is used
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request, int transacId = -1);
  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  DbConfiguration::db_type_t
  getDbType() { return DbConfiguration::MOCK; };
/**
 * \brief Start a transaction
 * \return The transaction ID
 */
  virtual int
  startTransaction();
/**
 * \brief End a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  endTransaction(int transactionID);
/**
 * \brief Cancel a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  cancelTransaction(int transactionID);
/**
 * \brief To commit a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  flush(int transactionID);
/**
 * \brief To get a unique id
 * \param table: The table to use to generate the id
 * \param fields: The fields of the table
 * \param val: The values of the fields to insert
 * \param tid: The transaction id
 * \return A new integer never returned by this function
 */
  virtual int
  generateId(std::string table, std::string fields, std::string val, int tid, std::string primary);

 /**
  * @brief escapeMySQLData : transform a sql data to a SQL-escaped string for MySQL
  * @param data: the string to transform
  * @return a espaced string
  */
  virtual std::string
  escapeData(const std::string& data);

private :
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The configuration of the database client
   */
  DbConfiguration mconfig;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
   * \brief To disconnect from the database
   * \fn disconnect()
   * \return 0 on success, an error code otherwise
   */
  int
  disconnect();
};


#endif // MockDATABASE
