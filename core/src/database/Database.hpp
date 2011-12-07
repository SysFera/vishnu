/**
 * \file Database.hpp
 * \brief This file presents an abstract database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _ABSTRACTDATABASE_H_
#define _ABSTRACTDATABASE_H_

#include <string>
#include <vector>
#include "DatabaseResult.hpp"
#include "DbConfiguration.hpp"

static const int SUCCESS =  0;
/**
 * \class Database
 * \brief This class describes a database
 */
class Database{
public :
  /**
   * \brief Function to process the request in the database
   * \fn    virtual int process(std::string request) = 0
   * \param request The request to process (must contain a SINGLE SQL statement without a semicolumn)
   * \param transacId the id of the transaction if one is used
   * \return raises an exception on error
   */
  virtual int
  process(std::string request, int transacId = -1) = 0;
  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  virtual int
  connect() = 0;
  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult(std::string request)
  * \param request The request to process
  * \param transacId the id of the transaction if one is used
  * \return An object which encapsulates the database results
  */
  virtual DatabaseResult*
  getResult(std::string request, int transacId = -1) = 0;
  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  virtual DbConfiguration::db_type_t
  getDbType() = 0;

    /**
   * \fn ~Database()
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
protected :
  /**
   * \fn Database()
   * \brief Constructor, raises an exception on error
   */
  Database();

private :
  /**
   * \brief To disconnect from the database
   * \fn virtual int disconnect() = 0
   * \return raises an exception on error
   */
  virtual int
  disconnect() = 0;
};


#endif // ABSTRACTDATABASE
