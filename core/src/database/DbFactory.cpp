/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include <iostream>
#include "DbFactory.hpp"
#include "SystemException.hpp"
#include "POSTGREDatabase.hpp"
//#include "OracleDatabase.hpp"

Database* DbFactory::mdb = NULL;

DbFactory::DbFactory(){
}

DbFactory::~DbFactory(){
}

Database*
DbFactory::createDatabaseInstance(DbConfiguration config)
{
  if (mdb != NULL) {
    throw SystemException(ERRCODE_DBERR, "Database instance already initialized");
  }
  switch (config.getDbType()){
    case DbConfiguration::POSTGRESQL :
      mdb = new POSTGREDatabase(config.getDbHost(),
                                config.getDbUserName(),
                                config.getDbUserPassword(),
                                config.getDbName(),
                                config.getDbPort()
                               );
      break;
    case DbConfiguration::ORACLE:
    case DbConfiguration::MYSQL:
    default:
      throw SystemException(ERRCODE_DBERR, "Database instance type unknown or not managed");
  }
  return mdb;
}

Database* DbFactory::getDatabaseInstance()
{
  if (mdb == NULL) {
    throw SystemException(ERRCODE_DBERR, "Database instance not initialized");
  }
  return mdb;
}

