/**
 * \file POSTGREDatabase.hh
 * \brief This file presents a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _POSTGREDATABASE_H_
#define _POSTGREDATABASE_H_

#include "Database.hh"
#include "libpq-fe.h"

/**
 * \class POSGREDatabase
 * \brief POSTRE implementation of the Database class
 */
class POSTGREDatabase : public Database {
public :
  /**
   * \brief Function to process the request in the database
   * \fn    void process(std::string request)
   * \param request The request to process
   * \return raises an exception on error
   */
  void 
  process(std::string request);
  /**
   * \brief To start a transaction with the database
   * \fn void startTransaction(std::string request)
   * \param request The series of requests to process
   * \return raises an exception on error
   */
  void 
  startTransaction(std::string request);
  /**
  * \brief To make a connection to the database
  * \fn void connect()
  * \return raises an exception on error
  */
  void 
  connect();
  /**
   * \fn POSTGREDatabase(std::string hostname,
   *		         std::string username,
   * 	 	         std::string pwd,
   *	 	         std::string database = "",
   *	 	         unsigned int port = 0)
   * \param hostname The name of the host to connect
   * \param username The username to connect to the database
   * \param pwd      The password to connect to the database
   * \param database The database to connect
   * \param port     The port to use
   * \brief Constructor, raises an exception on error
   */
  POSTGREDatabase(std::string hostname,
		  std::string username,
		  std::string pwd,
		  std::string database = "",
		  unsigned int port = 0);
  /**
   * \fn ~POSTGREDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~POSTGREDatabase();
  /**
   * \brief To commit a postgresql transaction
   * \fn void commit()
   * \return raises an exception on error
   */
  void
  commit ();
  /**
   * \brief To set the db to use
   * \fn void setDatabase(std::string db)
   * \param db The name of the database to use
   * \return 0 raises an exception on error
   */
  void
  setDatabase(std::string db);
  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult()
  * \param request The request to process
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request);
  /**
  * \brief To cancel a transaction
  * \fn void rollback()
  * \return raises an exception on error
  */
  void
  rollback();

private :
  
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  
  /**
  * \brief A PostgreSQL structure
  */
  PGconn* conn;
  /**
   * \brief The host of the base
   */
  std::string mhost;
  /**
   * \brief The username to connect
   */
  std::string musername;
  /**
   * \brief The password to connect
   */
  std::string mpwd;
  /**
   * \brief The database to connect
   */
  std::string mdatabase;
  /**
   * \brief The port to use
   */
  int mport;
  /**
   * \brief If the connection is right
   */
  bool misConnected;
  /**
   * \brief The current transaction
   */
  std::string SQLtransaction;
  
  /////////////////////////////////
  // Functions
  /////////////////////////////////
  
  /**
   * \brief To disconnect from the database
   * \fn void disconnect()
   * \return raises an exception on error
   */
  void
  disconnect();
};


#endif // POSTGREDATABASE
