/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include <iostream>
#include "DbFactory.hpp"
#include "SystemException.hpp"
#include "SOCIDatabase.hpp"

SOCIDatabase* DbFactory::mdb = NULL; //%RELAX<MISRA_0_1_3> Used in this file

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

	mdb=new SOCIDatabase(config);

  return mdb;
}

SOCIDatabase* DbFactory::getDatabaseInstance()
{
  if (mdb == NULL) {
    throw SystemException(ERRCODE_DBERR, "Database instance not initialized");
  }
  return mdb;
}

