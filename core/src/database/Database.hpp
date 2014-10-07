/**
 * \file Database.hpp
 * \brief This file presents an abstract database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _ABSTRACTDATABASE_H_
#define _ABSTRACTDATABASE_H_

#include <string>
#include <stdint.h>
#include "DatabaseResult.hpp"
#include "DbConfiguration.hpp"

/**
 * \class Database
 * \brief This class describes a database
 */
class Database{
public :
  /**
   * \brief Function to process the request in the database.
   * \param request The request to process (must contain a SINGLE SQL statement without a semicolumn)
   * \param transacId the id of the transaction if one is used
   * \return A pair<int, int> containing the return code and LAST_INSERT_ID (only useful for insertion). Raises an exception on error
   */
  virtual std::pair<int, uint64_t>
  process(const std::string& request, int transacId = -1) = 0;
  /**
  * \brief To make a connection to the database
  * \return raises an exception on error
  */
  virtual int
  connect() = 0;
  /**
  * \brief To get the result of a select request
  * \param request The request to process
  * \param transacId the id of the transaction if one is used
  * \return An object which encapsulates the database results
  */
  virtual DatabaseResult*
  getResult(const std::string& request, int transacId = -1) = 0;
  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  virtual DbConfiguration::db_type_t
  getDbType() = 0;

  /**
     * \brief Destructor, raises an exception on error
     */
  virtual ~Database();
  /**
 * \brief Start a transaction
 * \return The transaction ID
 */
  virtual int
  startTransaction() = 0;
  /**
 * \brief End a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  endTransaction(int transactionID) = 0;
  /**
 * \brief Cancel a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  cancelTransaction(int transactionID) = 0;
  /**
 * \brief To commit a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  flush(int transactionID) = 0;

  /**
   * @brief escapeMySQLData : transform a sql data to a SQL-escaped string for MySQL
   * @param data: the string to transform
   * @return a espaced string
   */
  virtual std::string escapeData(const std::string& data) = 0;


protected :
  /**
   * \brief Constructor, raises an exception on error
   */
  Database();

  /**
   * @brief Return the last inserted id
   * @param transactionId The transaction id
   * @param errorMsg OUT old error message in case of error
   * @return the id or -1 in case of error.
   */
  virtual int
  lastInsertedId(int transactionId, std::string& errorMsg) = 0;

private :
  /**
   * \brief To disconnect from the database
   * \return raises an exception on error
   */
  virtual int
  disconnect() = 0;
};


#endif // ABSTRACTDATABASE
