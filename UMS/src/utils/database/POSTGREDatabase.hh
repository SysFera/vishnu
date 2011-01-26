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
class POSTGREDatabase : public Database{
public :
  static const int DATABASE_NOT_CONNECTED = -1;
  static const int DATABASE_PROCESS_ERROR = -2;
  static const int DATABASE_ERROR = -3;
  
  /**
   * \brief Function to process the request in the database
   * \fn    int createDatabase()
   * \param request The request to process
   * \return 0 on success, an error code otherwise
   */
  int 
  process(std::string request);
  /**
   * \brief To start a transaction with the database
   * \fn virtual reconnect() = 0
   * \return 0 on success, an error code otherwise
   */
  int 
  startTransaction(std::string request);

  /**
   * \brief To reconnect to the database
   * \fn reconnect()
   * \return 0 on success, an error code otherwise
   */
   int 
  reconnect();
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
   * \brief To commit a mysql transaction
   * \fn virtual int commit()
   * \return 0 on success, an error code otherwise
   */
  int
  commit ();
  /**
   * \brief To set the db to use
   * \fn virtual int setDb(std::string db)
   * \param db The database to use
   * \return 0 on success, an error code otherwise
   */
  int
  setDatabase(std::string db);
  /**
   * \brief To get the result of the latest request (if any result)
   * \fn std::vector<std::string> getResult()
   * \return The result of the latest request
   */
  std::vector<std::string>
  getResult();
  /**
   * \brief To cancel a transaction
   * \fn int rollback()
   * \return 0 on success, an error code otherwise
   */
  int
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
   /**
   * \brief Store the results of the last request
   */
  std::vector<std::string> mres;
  /**
   * \brief Encapsulate the results of the last request
   */
  PGresult* res;
  
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


#endif // POSTGREDATABASE
