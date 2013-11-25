/**
 * \file POSTGREDatabase.hpp
 * \brief This class is used to access a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _POSTGREDATABASE_H_
#define _POSTGREDATABASE_H_

#include <string>
#include <pthread.h>

#include "Database.hpp"
#include "DatabaseResult.hpp"
#include "DbConfiguration.hpp"
#include "PGSQLRequestFactory.hpp"

#include "libpq-fe.h"

/**
 * \class POSTGREDatabase
 * \brief POSTGRESQL implementation of the Database class
 */
class POSTGREDatabase : public Database {
public :
  /**
   * \brief Function to process the request in the database
   * \param request The request to process
   * \param transacId the id of the transaction if one is used
   * \return raises an exception on error
   */
  int
  process(std::string request, int transacId = -1);


  /**
  * \brief To make a connection to the database
  * \return raises an exception on error
  */
  int
  connect();

  /**
   * \brief Constructor, raises an exception on error
   */
  POSTGREDatabase(DbConfiguration dbConfig);

  /**
   * \brief Destructor, raises an exception on error
   */
  ~POSTGREDatabase();

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
  getDbType() { return DbConfiguration::POSTGRESQL; };
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
  cancelTransaction(int transactionID) ;
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
 * \param primary the primary key on the table
 * \return A new integer never returned by this function
 */
  virtual int
  generateId(std::string table, std::string fields, std::string val, int tid, std::string primary);
  /**
 * \brief To get a request from a request file based on a key
 * \param key the key indicating the request to get
 * \return the corresponding sql request
 */
  virtual std::string
  getRequest(const int key);

  /**
   * @brief escapeData : transform a sql data to a SQL-escaped string for MySQL
   * @param data: the string to transform
   * @return a espaced string
   */
  virtual std::string
  escapeData(const std::string& data);

private :

  /**
   * \brief An element of the pool
   */
  typedef struct pool_t{
    /**
     * \brief If the connexion is used
     */
    bool mused;
    /**
     * \brief The connexion
     */
    PGconn* mconn;
    /**
     * \brief The associated mutex
     */
    pthread_mutex_t mmutex;
  }pool_t;

  /**
   * \brief To get a valid connexion
   * \param pos The position of the connexion gotten in the pool
   * \return A valid and free connexion
   */
  PGconn* getConnection(int& pos);

  /**
   * \brief To release a connexion
   * \param pos The position of the connexion to release
   */
  void releaseConnection(int pos);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
   * \brief The configuration of the database server
   */
  DbConfiguration mconfig;

  /**
   * \brief The pool of connection
   */
  pool_t *mpool;

  /**
   * \brief If the connection is right
   */
  bool misConnected;
  /**
   * \brief Request factory
   */
  PGSQLRequestFactory mpgfact;

  /////////////////////////////////
  // Functions
  /////////////////////////////////

  /**
   * \brief To disconnect from the database
   * \return raises an exception on error
   */
  int
  disconnect();
};


#endif // POSTGREDATABASE
