/**
 * \file DbFactory.hh
 * \brief This file defines the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _DBFACTORY_H_
#define _DBFACTORY_H_

#include "Database.hh"

/**
 * \brief Constant used for creating mysql databases
 */
static const int MYSQLDB   = 1;
/**
 * \brief Constant used for creating postgre/sql databases
 */
static const int POSTGREDB = 2;

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
   * \fn   Database* create(int type)
   * \param type The type of database to create
   * \return A database or a nil pointer
   */
  Database*
  create(int type, std::string host, std::string user, std::string pwd, std::string db = "", unsigned int port = 0);

};


#endif // _DBFACTORY_H_
