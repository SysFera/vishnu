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

static const int SUCCESS =  0;
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
  virtual int 
  process(std::string request) = 0;
  /**
   * \brief To start a transaction with the database
   * \fn int startTransaction(std::string request)
   * \param request The series of requests to process
   * \return raises an exception on error
   */
  virtual int 
  startTransaction(std::string request) = 0;
  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  virtual int 
  connect() = 0;
  /**
   * \brief To commit a transaction
   * \fn virtual int commit() = 0
   * \return raises an exception on error
   */
  virtual int
  commit () = 0;
  /**
   * \brief To set the name of the database to use
   * \fn virtual int setDatabase(std::string db) = 0
   * \param db The name of database to use
   * \return raises an exception on error
   */
  virtual int
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
  * \fn int rollback()
  * \return raises an exception on error
  */
  virtual int
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
   * \fn virtual int disconnect() = 0
   * \return raises an exception on error
   */
  virtual int 
  disconnect() = 0;
};


#endif // ABSTRACTDATABASE
