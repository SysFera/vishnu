/**
 * \file MYSQLDatabase.hh
 * \brief This file presents a SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _MYSDLDATABASE_H_
#define _MYSQLDATABASE_H_

#include "Database.hh"
#include "mysql.h"

/**
 * \class MYSQLDatabase
 * \brief MYSQL implementation of the Database
 */
class MYSQLDatabase : public Database{
public :
  static const int DATABASENOTCONNECTED = -1;
  /**
   * \brief Function to process the request in the database
   * \fn    int createDatabase()
   * \param request The request to process
   * \return 0 on success, an error code otherwise
   */
  int 
  process(std::string request);
  /**
   * \brief To reconnect to the database
   * \fn reconnect()
   * \return 0 on success, an error code otherwise
   */
  int 
  reconnect();
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
   * \brief To commit a mysql transaction
   * \fn virtual int commit()
   * \return 0 on success, an error code otherwise
   */
  int
  commit ();
  /**
   * \brief To set the db to use
   * \fn int setDb(std::string db)
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
   * \brief A MYSQL structure
   */
  MYSQL mmysql;
  /**
   * \brief Store the results of the last request
   */
  std::vector<std::string> mres;

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
