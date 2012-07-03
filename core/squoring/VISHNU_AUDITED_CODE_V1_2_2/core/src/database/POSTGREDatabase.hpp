/**
 * \file POSTGREDatabase.hpp
 * \brief This class is used to access a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _POSTGREDATABASE_H_
#define _POSTGREDATABASE_H_

#include "SOCIDatabase.hpp"
#include "DbConfiguration.hpp"
#include "libpq-fe.h"
#include <pthread.h>

/**
 * \class POSTGREDatabase
 * \brief POSTGRESQL implementation of the Database class
 */
class POSTGREDatabase : public SOCIDatabase {
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
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  int
  connect();

  /**
   * \fn POSTGREDatabase(DbConfiguration dbConfig)
   * \brief Constructor, raises an exception on error
   */
  POSTGREDatabase(DbConfiguration dbConfig);

  /**
   * \fn ~POSTGREDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~POSTGREDatabase();

  /**
   * FIXME obsolete
   * \brief To set the db to use
   * \fn int setDatabase(std::string db)
   * \param db The name of the database to use
   * \return 0 raises an exception on error
   */
  int
  setDatabase(std::string db) {}

  /**
  * \brief To get the result of a select request
  * \fn DatabaseResult* getResult(std::string request)
  * \param request The request to process
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request);

  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  DbConfiguration::db_type_t
  getDbType() { return DbConfiguration::POSTGRESQL; };

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
   * \fn PGconn* getConnexion(int& pos)
   * \param pos The position of the connexion gotten in the pool
   * \return A valid and free connexion
   */
  PGconn* getConnexion(int& pos);

  /**
   * \brief To release a connexion
   * \fn void releaseConnexion(int pos)
   * \param pos The position of the connexion to release
   */
  void releaseConnexion(int pos);

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
