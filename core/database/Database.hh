/**
 * \file Database.hh
 * \brief This file presents an abstract database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10 
 */

#ifndef _ABSTRACTDATABASE_H_
#define _ABSTRACTDATABASE_H_

#include <string>
#include <vector>
#include "DatabaseResult.hh"

/**
 * \class Database
 * \brief This class describes a database
 */
class Database{
public :
  /**
   * \brief Function to process the request in the database
   * \fn    virtual int createDatabase() = 0
   * \param request The request to process
   * \return raises an exception on error
   */
  virtual void 
  process(std::string request) = 0;
  /**
   * \brief To start a transaction with the database
   * \fn void startTransaction(std::string request)
   * \param request The series of requests to process
   * \return raises an exception on error
   */
  virtual void 
  startTransaction(std::string request) = 0;
  /**
  * \brief To make a connection to the database
  * \fn void connect()
  * \return raises an exception on error
  */
  virtual void 
  connect() = 0;
  /**
   * \brief To commit a transaction
   * \fn virtual void commit() = 0
   * \return raises an exception on error
   */
  virtual void
  commit () = 0;
  /**
   * \brief To set the name of the database to use
   * \fn virtual void setDatabase(std::string db) = 0
   * \param db The name of database to use
   * \return raises an exception on error
   */
  virtual void
  setDatabase(std::string db) = 0;
  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult()
  * \param request The request to process
  * \return An object which encapsulates the database results
  */
  virtual DatabaseResult*
  getResult(std::string request) = 0;
  /**
  * \brief To cancel a transaction
  * \fn void rollback()
  * \return raises an exception on error
  */
  virtual void
  rollback() = 0;
protected :
  /**
   * \fn Database()
   * \brief Constructor, raises an exception on error
   */
  Database();
  /**
   * \fn ~Database()
   * \brief Destructor, raises an exception on error
   */
virtual ~Database();
private :
  /**
   * \brief To disconnect from the database
   * \fn virtual void disconnect() = 0
   * \return raises an exception on error
   */
  virtual void 
  disconnect() = 0;
};


#endif // ABSTRACTDATABASE
