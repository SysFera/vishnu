/**
 * \file POSTGREDatabase.cc
 * \brief This file implements a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com) and Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/10
 */
#include <sstream>
#include <iostream>
#include "POSTGREDatabase.hh"

/**
 * \brief Function to process the request in the database
 * \fn    int createDatabase()
 * \param request The request to process
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::process(std::string request){
    
 if (misConnected) {
     
    res = PQexec(conn, request.c_str());   
    int nFields = PQnfields(res);
    //int ntuples
 
     int i;
     int j;
    
    for (i = 0; i < nFields; i++) {
        std::cout<<"Attribut name:"<<PQfname(res, i)<<std::endl;
    }
    
    
    for (i = 0; i < PQntuples(res); i++) {
      for (j = 0; j < nFields; j++) {
	    std::cout<<"Mon test:"<<std::string(PQgetvalue(res, i, j))<<std::endl;
      }
   }
     
    PQclear(res);
    
   } //End if misConnected
   
  else {
    // TODO THROW AN EXCEPTION
  return DATABASE_NOT_CONNECTED;
  }
  
  return 0;
}

/**
* \brief To start a transaction with the database
* \fn virtual startTransaction(std::string request)
* \return 0 on success, an error code otherwise
*/
int 
POSTGREDatabase::startTransaction(std::string request){
 if (misConnected)
  return DATABASE_NOT_CONNECTED; 

  SQLtransaction.clear();
  SQLtransaction = "BEGIN;";
  SQLtransaction.append(request);
  return 0;
}

/**
 * \brief To reconnect to the database
 * \fn reconnect()
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::reconnect(){
  return 0;
}

/**
 * \fn Database()
 * \brief Constructor, raises an exception on error
 */
POSTGREDatabase::POSTGREDatabase(std::string hostname,
		  std::string username,
		  std::string pwd,
		  std::string database,
		  unsigned int port):Database(){
		    
  mhost        = hostname;
  musername    = username;
  mpwd         = pwd;
  mdatabase    = database;
  misConnected = false;
  mport        = port;  
  
  
  // IF MPORT < 0 TODO THROW AN EXCEPTION
  if (!misConnected) {
     std::ostringstream out;
     out<<mport;
     std::cout<<"mport"<<out.str()<<std::endl;

     
      /* Make a connection to the database */
  conn = PQsetdbLogin(mhost.c_str(),
                      "",
                      "",
                      mpwd.c_str(),
                      mdatabase.c_str(),
                      musername.c_str(),
                      out.str().c_str());		     
     
      if (PQstatus(conn) != CONNECTION_OK)
	{
	    fprintf(stderr, "Connection to database failed: %s",
		    PQerrorMessage(conn));
	    disconnect(); //TODO THROW AN EXCEPTION
	    //return DATABASE_NOT_CONNECTED;
	}
	// TODO THROW AN EXCEPTION
    misConnected = true;
    std::cout<<"The connection to the database "<<mdatabase
    <<" "<<"is successfully completed"<<std::endl; 
  } else {
    // TODO THROW AN EXCEPTION Database already connected
  }  
    
}
/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
POSTGREDatabase::~POSTGREDatabase(){
  disconnect();
}


/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::disconnect(){
  if (!misConnected)
    return DATABASE_NOT_CONNECTED;
    PQfinish(conn);
  return 0;
}

/**
 * \brief To commit a mysql transaction
 * \fn virtual int commit()
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::commit (){
  if (misConnected)
  return DATABASE_NOT_CONNECTED; 
  SQLtransaction.append("COMMIT;");//TODO 
  return 0;
}

/**
 * \brief To set the db to use
 * \fn virtual int setDb(std::string db)
 * \param db The database to use
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::setDatabase(std::string db){
  return 0;
}

/**
 * \brief To get the result of the latest request (if any result)
 * \fn std::vector<std::string> getResult()
 * \return The result of the latest request
 */
std::vector<std::string>
POSTGREDatabase::getResult(){
  
  if (!misConnected)
    return std::vector<std::string>();
  
  int nFields;
  int i;
  int j;

  std::string tmp;
  mres.clear();
 
  
  /* first, print out the attribute names */
    nFields = PQnfields(res);
  
  for (i = 0; i < PQntuples(res); i++) {
    tmp.clear();
    for (j = 0; j < nFields; j++) {
	    tmp += std::string(PQgetvalue(res, i, j));
    }
     mres.push_back(tmp); 
   }
  
  //PQclear(res);
  return mres;
}

/**
 * \brief To cancel a transaction
 * \fn int rollback()
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::rollback(){
  if (misConnected)
  return DATABASE_NOT_CONNECTED; 
  SQLtransaction.append("ROLLBACK;"); //TODO 
  return 0;
}

