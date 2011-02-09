/**
 * \file POSTGREDatabase.cc
 * \brief This file implements a PostGreSQL database.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2011
 */
#include <sstream>
#include <iostream>
#include "POSTGREDatabase.hpp"
#include "SystemException.hpp"

/**
 * \brief Function to process the request in the database
 * \fn    int process(std::string request)
 * \param request The request to process
 * \return raises an exception on error
 */
int 
POSTGREDatabase::process(std::string request){
 
 PGresult* res;
 
 if (PQstatus(mconn) == CONNECTION_OK) {
    res = PQexec(mconn, request.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	PQclear(res);
	std::string errorMsg = std::string(PQerrorMessage(mconn));
	errorMsg.append("- Note: The process function must not be used for select request");
	SystemException e(2, errorMsg);
	throw e;
    }
    PQclear(res);
 } //END if CONNECTION_OK
 else {
   SystemException e(2, "The database is not connected");
   throw e;
   }
 return SUCCESS;
}// END int process(std::string request)

/**
 * \brief To start a transaction with the database
 * \fn int startTransaction(std::string request)
 * \param request The series of requests to process
 * \return raises an exception on error
 */
  
int 
POSTGREDatabase::startTransaction(std::string request){
  
 PGresult* res;
  
 if (PQstatus(mconn) == CONNECTION_OK) {  
   mSQLtransaction.clear();
   mSQLtransaction.append(request);
  
    res = PQexec(mconn, "BEGIN TRANSACTION;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	PQclear(res);
	SystemException e(2, std::string(PQerrorMessage(mconn)));
	throw e;
    }
    PQclear(res);
 
    res = PQexec(mconn, mSQLtransaction.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
	SystemException e(2, std::string(PQerrorMessage(mconn)));
	throw e;
    }
    PQclear(res);
  
 } //END if CONNECTION_OK
 else {
    SystemException e(2, "The database is not connected");
    throw e;
 }
 return SUCCESS;
}//END int startTransaction(std::string request)

/**
 * \brief To make a connection to the database
 * \fn int connect()
 * \return raises an exception on error
 */
int 
POSTGREDatabase::connect(){

  if (PQstatus(mconn) != CONNECTION_OK) {
     std::ostringstream out;
     out << mport;
    
     if (mport < 0) {
	SystemException e(2, "The port value is incorrect");
	throw e; 
     }
  
     /* Make a connection to the database */
     mconn = PQsetdbLogin(mhost.c_str(),
			  "",
			  "",
			  out.str().c_str(),
			  mdatabase.c_str(),
			  musername.c_str(),
			  mpwd.c_str());		     
	
     if (PQstatus(mconn) != CONNECTION_OK) {
	 SystemException e(2, std::string(PQerrorMessage(mconn)));
	 throw e;
     }
     misConnected = true;
     
  }//END if NOT CONNECTION_OK 
  else {
    SystemException e(2, "connect : The database is already connected");
    throw e;
  }
 return SUCCESS;
}//END int connect()

/**
 * \fn Database()
 * \brief Constructor, raises an exception on error
 */
POSTGREDatabase::POSTGREDatabase(std::string hostname,
		  std::string username,
		  std::string pwd,
		  std::string database,
		  unsigned int port):Database(){
		     
  if (port < 0) {
    SystemException e(2, "The port value is incorrect");
    throw e; 
  }
 
 mhost        = hostname;
 musername    = username;
 mpwd         = pwd;
 mdatabase    = database;
 misConnected = false;
 mport        = port;     
 mconn         = NULL;
 std::cout << "Constructed " << std::endl;
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
 * \return 0 raises an exception on error
 */
int 
POSTGREDatabase::disconnect(){
  if (mconn != NULL) {
    PQfinish(mconn);
  }
 return SUCCESS;
}

/**
 * \brief To commit a To commit a postgresql transaction transaction
 * \fn int commit()
 * \return raises an exception on error
 */
int
POSTGREDatabase::commit (){
    
 PGresult* res;
 
 if (PQstatus(mconn) == CONNECTION_OK) { 
    res = PQexec(mconn, "COMMIT TRANSACTION;");
      
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	  PQclear(res);
	  SystemException e(2, std::string(PQerrorMessage(mconn)));
	  throw e;
    }
    PQclear(res);
 } //IF CONNECTION_OK
 else {
    SystemException e(2, "commit : the database is not connected");
    throw e;
 }  
 return SUCCESS;
}

/**
 * \brief To set the db to use
 * \fn int setDatabase(std::string db)
 * \param db The name of database to use
 * \return raises an exception on error
 */
int
POSTGREDatabase::setDatabase(std::string db){
  
  if (PQstatus(mconn) != CONNECTION_OK) {
    mdatabase = db;
  }
  else {
    SystemException e(2, "The database is connected! Imposssible to set a new database name");
    throw e;
  } 
 return SUCCESS;
}

/**
 * \brief To get the result of a select request
 * \fn DatabaseResult* getResult()
 * \param request The request to process
 * \return An object which encapsulates the database results
 */
DatabaseResult*
POSTGREDatabase::getResult(std::string request) {
 
 PGresult* res;
 std::vector<std::vector<std::string> > results;
 std::vector<std::string> attributesNames;
 std::vector<std::string> tmp;
 int nFields;
 int i;
 int j;
 
 if (PQstatus(mconn) == CONNECTION_OK) {
   
    res = PQexec(mconn, request.c_str());    
     
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
	  PQclear(res);
	  SystemException e(2, std::string(PQerrorMessage(mconn)));
	  throw e;
    }
    
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++) {
	attributesNames.push_back(std::string(PQfname(res, i)));
    }
    
    for (i = 0; i < PQntuples(res); i++) {
	tmp.clear();
	for (j = 0; j < nFields; j++) {
	      std::cout<<"Mon test:"<<std::string(PQgetvalue(res, i, j))<<std::endl;
	      tmp.push_back(std::string(PQgetvalue(res, i, j)));
	}
	results.push_back(tmp);
    }
   
   return new DatabaseResult(results, attributesNames);
 } //End if CONNECTION_OK
   
 else {
    SystemException e(2, "getResult : the database is not connected");
    throw e;
 }
 return SUCCESS; 
}

/**
 * \brief To cancel a transaction
 * \fn int rollback()
 * \return raises an exception on error
 */
int
POSTGREDatabase::rollback(){
    
  PGresult* res;
    
  if (PQstatus(mconn) == CONNECTION_OK) {
      res = PQexec(mconn, "ROLLBACK;");
      if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	  PQclear(res);
	  SystemException e(2, std::string(PQerrorMessage(mconn)));
	  throw e;
      }
      PQclear(res);   
   } 
   else {
	SystemException e(2, "rollback : the database is not connected");
	throw e;
   }
      
 return SUCCESS;
}

