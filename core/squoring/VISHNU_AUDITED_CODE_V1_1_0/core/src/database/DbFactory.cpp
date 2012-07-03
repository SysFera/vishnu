/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include <iostream>
#include "DbFactory.hpp"
#include "SystemException.hpp"
#ifdef USE_POSTGRES
#include "POSTGREDatabase.hpp"
#endif
#ifdef USE_MYSQL
#include "MYSQLDatabase.hpp"
#endif
//#include "OracleDatabase.hpp"

SOCIDatabase* DbFactory::mdb = NULL;

DbFactory::DbFactory(){
}

DbFactory::~DbFactory(){
}

SOCIDatabase*
DbFactory::createDatabaseInstance(DbConfiguration config)
{
  if (mdb != NULL) {
    throw SystemException(ERRCODE_DBERR, "Database instance already initialized");
  }
  switch (config.getDbType()){
    case DbConfiguration::POSTGRESQL :
#ifdef USE_POSTGRES
      mdb = new POSTGREDatabase(config.getDbHost(),
                                config.getDbUserName(),
                                config.getDbUserPassword(),
                                config.getDbName(),
                                config.getDbPort()
                               );
#endif
      break;
    case DbConfiguration::ORACLE:
      break;
    case DbConfiguration::MYSQL:
#ifdef USE_MYSQL
      mdb = new MYSQLDatabase(config.getDbHost(),
			      config.getDbUserName(),
			      config.getDbUserPassword(),
			      config.getDbName(),
			      config.getDbPort()
			      );
#endif
      break;
  default:
    throw SystemException(ERRCODE_DBERR, "Database instance type unknown or not managed");
  }
  return mdb;
}

SOCIDatabase* DbFactory::getDatabaseInstance()
{
  if (mdb == NULL) {
    throw SystemException(ERRCODE_DBERR, "Database instance not initialized");
  }
  return mdb;
}

