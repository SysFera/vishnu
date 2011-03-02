/**
 * \file OracleDatabase.hpp
 * \brief This file presents a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _ORACLEDATABASE_HH_
#define _ORACLEDATABASE_HH_

#include "InternalVishnuException.hpp"
#include "Database.hpp"
#include "occi.h"
#include <vector>

using namespace oracle::occi;
/**
 * \class OracleDatabase
 * \brief Oracle implementation of the Database class
 */
class OracleDatabase : public Database {
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
   * \fn OracleDatabase(std::string hostname,
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
  OracleDatabase(std::string hostname,
                 std::string username,
                 std::string pwd,
                 std::string database = "",
                 unsigned int port = 0);

  /**
   * \fn ~OracleDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~OracleDatabase();

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
  * \fn DatabaseResult* getResult(std::string request)
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
   * \brief The oracle environnement
   */
  Environment *menvironment;
  /**
   * \brief The connection structure
   */
  Connection  *mcon;
  /**
   * \brief The statement (=request)
   */
  Statement   *mstmt;
  /**
   * The result of a request
   */
  ResultSet   *mres;

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
