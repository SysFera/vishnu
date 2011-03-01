/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include <iostream>
#include "DbFactory.hpp"
#include "POSTGREDatabase.hpp"
//#include "OracleDatabase.hpp"

Database* DbFactory::mdb = NULL;

DbFactory::DbFactory(){
}

DbFactory::~DbFactory(){
}

Database*
DbFactory::getDatabaseInstance(int type, std::string host, std::string user, std::string pwd, std::string db, unsigned int port){

  if (mdb != NULL) {
    return mdb;
  }
  switch (type){
    case ORACLEDB:
      //mdb = new OracleDatabase(host, user, pwd, db, port);
      //break;
    case POSTGREDB :
      mdb = new POSTGREDatabase(host, user, pwd, db, port);
      break;
    default:
      mdb = NULL;
  }
  return mdb;
}
