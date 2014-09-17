/**
 * \file MYSQLDatabase.hpp
 * \brief This file presents a SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _MYSDLDATABASE_H_
#define _MYSQLDATABASE_H_

#include <string>
#include <pthread.h>
#include "Database.hpp"
#include "DatabaseResult.hpp"
#include "DbConfiguration.hpp"
#include "mysql.h"

/**
 * \class MYSQLDatabase
 * \brief MYSQL implementation of the Database
 */
class MYSQLDatabase : public Database{
public :
  /**
   * \brief Function to process the request in the database
   * \param query The SQL query to process
   * \param transacId the id of the transaction if one is used
   * \return A pair<int, int> containing the return code and LAST_INSERT_ID (only useful for insertion). Raises an exception on error
   */
  virtual std::pair<int, uint64_t>
  process(const std::string& query, int transacId = -1);
  /**
  * \brief To make a create a pool of MySQL connections
  * \return raises an exception on error
  */
  int
  connect();

  /**
  * \brief To make a connection to the database and store the return db handler to a given pool's index
  * \param poolIdx the index in connexion pool
  * \return raises an exception on error
  */
  void connectPoolIndex(const int& poolIdx);

  /**
   * \brief Constructor, raises an exception on error
   * \param dbConfig  the configuration of the database client
   */
  MYSQLDatabase(DbConfiguration dbConfig);

  /**
   * \brief Destructor, raises an exception on error
   */
  ~MYSQLDatabase();

  /**
  * \brief To get the result of a select request
  * \param query The request to process
  * \param transacId the id of the transaction if one is used
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(const std::string& query, int transacId = -1);

  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  DbConfiguration::db_type_t
  getDbType() { return DbConfiguration::MYSQL; }

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
   * @brief escapeData : transform a sql data to a SQL-escaped string for MySQL
   * @param data: the string to transform
   * @return a espaced string
   */
  virtual std::string
  escapeData(const std::string& data);


protected:
  /**
   * @brief Return the last inserted id
   * @param transactionId The transaction id
   * @param errorMsg OUT old error message in case of error
   * @return the id or -1 in case of error.
   */
  virtual int
  lastInsertedId(int transactionId, std::string& errorMsg);

private :
  /**
   * \brief To get a valid connexion
   * \param pos The position of the connection gotten in the pool
   * \return A valid and free connection
   */
  MYSQL* getConnection(int& pos);

  /**
   * \brief To release a connexion
   * \param pos The position of the connection to release
   */
  void releaseConnection(int pos);

  /**
   * \brief An element of the pool
   */
  typedef struct pool_t{
    /**
     * \brief If the connexion is used
     */
    bool mused;
    /**
     * \brief The connection mysql structure
     */
    MYSQL mmysql;
    /**
     * \brief The associated mutex
     */
    pthread_mutex_t mmutex;
  }pool_t;
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The configuration of the database client
   */
  DbConfiguration mconfig;
  /**
   * \brief The pool of connection
   */
  pool_t *mpool;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
   * \brief To disconnect from the database
   * \return 0 on success, an error code otherwise
   */
  int
  disconnect();


  /**
   * @brief Get a connection from the pool of connections
   * @param transaction The id of the related transaction
   * @return Pair of connection and index in the pool
   */
  std::pair<MYSQL*, int>
  getConnectionFromPool(int transaction);

  /**
   * @brief Raise exception if MySQL exited with critical error
   * @param conn The MYSQL connection pointer
   * @param poolIndex The index of the connection in the pool
   */
  void
  raiseOnCriticalMysqlError(MYSQL* conn, int poolIndex);

  /**
   * @brief Raise exception if a MySQL call exit with non-zero error code
   * @param rc MySQL api return code
   * @param conn The MYSQL connection pointer
   * @param poolIndex The index of the connection in the pool
   */
  void
  raiseExceptionIfMysqlError(int ecode, MYSQL* conn, int poolIndex);
};


#endif // MYSQLDATABASE
