/**
 * \file DbFactory.cc
 * \brief This file implements the database factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include <stdio.h>
#include "DbFactory.hh"
#include <iostream>
//#include "MYSQLDatabase.hh"

//#include "mysql.h"

//class MYSQLDatabase;
//class POSTGREDatabase;

Database* DbFactory::mdb = NULL;

DbFactory::DbFactory(){

}

DbFactory::~DbFactory(){

}


Database*
DbFactory::getDatabaseInstance(int type, std::string host, std::string user, std::string pwd, std::string db, unsigned int port){
  
  if (mdb != NULL) {
    std::cout << "Mon Sigleton existe" << std::endl;
    //<< "Mon Sigleton existe";  
    return mdb;
  }
  std::cout <<"Mon Sigleton n'existe pas"<< std::endl;
  switch (type){
  /*case MYSQLDB :
    return  new MYSQLDatabase (host, user, pwd, db, port);
    break;*/
  case POSTGREDB :
  	mdb = new POSTGREDatabase (host, user, pwd, db, port);
	return mdb;   
    break;
  }
  return NULL;
}
