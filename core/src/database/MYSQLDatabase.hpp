/**
 * \file MYSQLDatabase.hpp
 * \brief This file presents a SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _MYSDLDATABASE_H_
#define _MYSQLDATABASE_H_

#include "Database.hpp"
#include "mysql.h"

/**
 * \class MYSQLDatabase
 * \brief MYSQL implementation of the Database
 */
class MYSQLDatabase : public Database{
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
   * \fn MYSQLDatabase(DbConfiguration dbConfig)
   * \param dbConfig  the configuration of the database client
   */
  MYSQLDatabase(DbConfiguration dbConfig);
  /**
   * \fn ~MYSQLDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~MYSQLDatabase();
  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult(std::string request)
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
  getDbType() { return DbConfiguration::MYSQL; };
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

private :
  /**
   * \brief To get a valid connexion
   * \fn MYSQL* getConnection(int& pos)
   * \param pos The position of the connection gotten in the pool
   * \return A valid and free connection
   */
  MYSQL* getConnection(int& pos);

  /**
   * \brief To release a connexion
   * \fn void releaseConnection(int pos)
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
   * \fn disconnect()
   * \return 0 on success, an error code otherwise
   */
  int
  disconnect();
};


#endif // MYSQLDATABASE
