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
   * \param request The request to process
   * \return 0 on success, an error code otherwise
   */
  int
  process(std::string request);
  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  int
  connect();
  /**
   * \brief Constructor, raises an exception on error
   * \fn MYSQLDatabase(std::string hostname,
   *          	       std::string username,
   *          	       std::string pwd,
   *	      	       std::string database = "",
   *	      	       unsigned int port = 0)
   * \param hostname The name of the host to connect
   * \param username The username to connect to the database
   * \param pwd      The password to connect to the database
   * \param database The database to connect
   * \param port     The port to use
   */
  MYSQLDatabase(std::string hostname,
		 std::string username,
		 std::string pwd,
		 std::string database = "",
		 unsigned int port = 0);
  /**
   * \fn ~MYSQLDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~MYSQLDatabase();
  /**
   * \brief To set the db to use
   * \param db The database to use
   * \return 0 on success, an error code otherwise
   */
  int
  setDatabase(std::string db);
  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult(std::string request)
  * \param request The request to process
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request);

private :
  /**
   * \brief To get a valid connexion
   * \fn MYSQL* getConnexion(int& pos)
   * \param pos The position of the connexion gotten in the pool
   * \return A valid and free connexion
   */
  MYSQL* getConnexion(int& pos);

  /**
   * \brief To release a connexion
   * \fn void releaseConnexion(int pos)
   * \param pos The position of the connexion to release
   */
  void releaseConnexion(int pos);
  /**
   * \brief Size of the pool
   */
  static const int MPOOLSIZE = 20;

  /**
   * \brief An element of the pool
   */
  typedef struct pool_t{
    /**
     * \brief If the connexion is used
     */
    bool mused;
    /**
     * \brief The connexion mysql structure
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
   * \brief The pool of connection
   */
  pool_t mpool[MPOOLSIZE];
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
