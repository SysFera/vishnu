/**
 * \file DbFactory.cc
 * \brief This file implements the database factory mock
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/13
 */

#include <iostream>
#include "DbFactory.hpp"
#include "MockDatabase.hpp"

Database* DbFactory::mdb = NULL; //%RELAX<MISRA_0_1_3> Used in this file

DbFactory::DbFactory(){
}

DbFactory::~DbFactory(){
}

Database*
DbFactory::createDatabaseInstance(DbConfiguration config)
{
  mdb = new MockDatabase(config);
  return mdb;
}

Database* DbFactory::getDatabaseInstance()
{
  return mdb;
}

