/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include "DbFactory.hh"
#include "MYSQLDatabase.hh"
#include "POSTGREDatabase.hh"
#include "mysql.h"

class MYSQLDatabase;

DbFactory::DbFactory(){

}

DbFactory::~DbFactory(){

}


Database*
DbFactory::create(int type, std::string host, std::string user, std::string pwd, std::string db, unsigned int port){
  switch (type){
  case MYSQLDB :
    return  new MYSQLDatabase (host, user, pwd, db, port);
    break;
  case POSTGREDB :
    return new POSTGREDatabase (host, user, pwd, db, port);
    break;
  }
  return NULL;
}
