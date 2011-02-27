/**
 * \file POSTGREDatabase.hpp
 * \brief This class is used to access a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _POSTGREDATABASE_H_
#define _POSTGREDATABASE_H_

#include "Database.hpp"
#include "libpq-fe.h"

/**
 * \class POSTGREDatabase
 * \brief POSTGRESQL implementation of the Database class
 */
class POSTGREDatabase : public Database {
public :
  /**
   * \brief Function to process the request in the database
   * \fn    int process(std::string request)
   * \param request The request to process
   * \return raises an exception on error
   */
  int
  process(std::string request);

  /**
   * \brief To start a transaction with the database
   * \fn int startTransaction(std::string request)
   * \param request The series of requests to process
   * \return raises an exception on error
   */
  int
  startTransaction(std::string request);

  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  int
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
   * \fn int commit()
   * \return raises an exception on error
   */
  int
  commit ();

  /**
   * \brief To cancel a transaction
   * \fn int rollback()
   * \return raises an exception on error
   */
  int
  rollback();

  /**
   * \brief To set the db to use
   * \fn int setDatabase(std::string db)
   * \param db The name of the database to use
   * \return 0 raises an exception on error
   */
  int
  setDatabase(std::string db);

  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult()
  * \param request The request to process
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request);


private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief A PostgreSQL structure
  */
  PGconn* mconn;
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
  std::string mSQLtransaction;

  /////////////////////////////////
  // Functions
  /////////////////////////////////

  /**
   * \brief To disconnect from the database
   * \fn int disconnect()
   * \return raises an exception on error
   */
  int
  disconnect();
};


#endif // POSTGREDATABASE
