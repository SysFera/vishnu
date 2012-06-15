/**
 * \file SOCIDatabase.hpp
 * \brief This file presents a SQL database.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 14/06/12
 */

#ifndef _SOCIDATABASE_H_
#define _SOCIDATABASE_H_

#include "Database.hpp"
#include <soci/soci.h>
#include <soci/soci-backend.h>
#include <vector>





/**
 * \class SOCIDatabase
 * \brief SOCI implementation of the Database
 */
class SOCIDatabase : public Database{
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
   * \fn SOCIDatabase(DbConfiguration dbConfig)
   * \param dbConfig  the configuration of the database client
   */
  SOCIDatabase(DbConfiguration dbConfig);
  /**
   * \fn ~SOCIDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~SOCIDatabase();
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
  getDbType() { return mdbtype; };
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
  generateId(std::string table, std::string fields, std::string val, int tid);

private :
  /**
   * \brief To get a valid connexion
   * \fn session & getConnection(int& pos)
   * \param pos The position of the connection gotten in the pool
   * \return A valid and free connection
   */
  soci::session & getConnection(int& pos);

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
     * \brief The connection SOCI structure
     */
    soci::session msession;
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
  soci::connection_pool* mpool;
  /**
   * \brief The configuration of the database client
   */
  DbConfiguration::db_type_t mdbtype;


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

  /*
   * \brief convert a SOCI row into a vector of string
   * \param the row to convert
   */
  std::vector<std::string>
  rowToString(const soci::row & r);
  /*
   * \brief convert
   */
  std::string
  dataToString(const soci::row & r, size_t pos);
};


#endif // SOCIDATABASE
