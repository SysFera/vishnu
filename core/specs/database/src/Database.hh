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
   * \return 0 on success, an error code otherwise
   */
  virtual int 
  process(std::string request) = 0;
  /**
   * \brief To reconnect to the database
   * \fn virtual reconnect() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int 
  reconnect() = 0;
  /**
   * \brief To commit a mysql transaction
   * \fn virtual int commit() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  commit () = 0;
  /**
   * \brief To set the db to use
   * \fn virtual int setDb(std::string db) = 0
   * \param db The database to use
   * \return 0 on success, an error code otherwise
   */
  virtual int
  setDatabase(std::string db) = 0;
  /**
   * \brief To get the result of the latest request (if any result)
   * \fn virtual void* getResult() = 0
   * \return The result of the latest request
   */
  virtual std::vector<std::string>
  getResult() = 0;
  /**
   * \brief To cancel a transaction
   * \fn virtual int rollback() = 0
   * \return 0 on success, an error code otherwise
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
  ~Database();
private :
  /**
   * \brief To disconnect from the database
   * \fn virtual disconnect() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int 
  disconnect() = 0;
};


#endif // ABSTRACTDATABASE
