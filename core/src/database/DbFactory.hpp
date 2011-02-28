/**
 * \file DbFactory.hpp
 * \brief This file defines the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _DBFACTORY_H_
#define _DBFACTORY_H_

#include "Database.hpp"

/**
 * \brief Constant used for creating mysql databases
 */
static const int MYSQLDB   = 1;
/**
 * \brief Constant used for creating postgre/sql databases
 */
static const int POSTGREDB = 2;
/**
 * \brief Constant used for creating oracle databases
 */
static const int ORACLEDB = 3;

/**
 * \class DbFactory
 * \brief Database factory
 */
class DbFactory{

public :
  /**
   * \brief Constructor
   * \fn DbFactory()
   */
  DbFactory();
  /**
   * \brief Destructor
   * \fn ~DbFactory()
   */
  ~DbFactory();
  /**
   * \brief Function to create a database. The database created depends on the type parameter
   * \fn   Database* create(int type = -1, std::string host = "", std::string user = "", std::string pwd = "", std::string db = "", unsigned int port = 0)
   * \param type The type of database to create
   * \return A database or a nil pointer
   */
  Database*
  getDatabaseInstance(int type = -1, std::string host = "", std::string user = "", std::string pwd = "", std::string db = "", unsigned int port = 0);

private :
  static Database* mdb;
};


#endif // _DBFACTORY_H_

