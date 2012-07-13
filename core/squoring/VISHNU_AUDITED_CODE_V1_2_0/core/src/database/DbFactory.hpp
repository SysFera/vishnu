/**
 * \file DbFactory.hpp
 * \brief This file defines the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#ifndef _DBFACTORY_H_
#define _DBFACTORY_H_

#include "Database.hpp"
#include "DbConfiguration.hpp"


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
   * \brief Function to create a database from a configuration
   * \param dbConfig  the configuration of the database
   * \return A database or a nil pointer
   */
  Database*
  createDatabaseInstance(DbConfiguration dbConfig);

  /**
   * \brief Get the single instance of the database
   * \return A database or a nil pointer
   */
  Database*
  getDatabaseInstance();

private :
  /**
   * \brief The unique instance of the database
   */
  static Database* mdb;
};


#endif // _DBFACTORY_H_

