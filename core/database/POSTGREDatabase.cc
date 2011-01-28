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
    
    
    
     std::vector<std::vector<std::string> > results;
     std::vector<std::string> testres;
    
    int nFields = PQnfields(res);
    //int ntuples
 
     int i;
     int j;
    
    for (i = 0; i < nFields; i++) {
        //std::cout<<"Attribut name:"<<PQfname(res, i)<<std::endl;
	testres.push_back(std::string(PQfname(res, i)));
    }
    results.push_back(testres);
    
    
    for (i = 0; i < PQntuples(res); i++) {
      testres.clear();
      for (j = 0; j < nFields; j++) {
	    std::cout<<"Mon test:"<<std::string(PQgetvalue(res, i, j))<<std::endl;
	    testres.push_back(std::string(PQgetvalue(res, i, j)));
      }
      results.push_back(testres);
   }
     
    PQclear(res);
    std::cout<<"Affichage des résultats"<<std::endl;
   
    /*for (i = 0; i < results.size(); ++i) {
      //testres.clear();
      testres = results[i];
      
       std::vector<std::string>::iterator ii;
 
	for(ii=testres.begin();ii!=testres.begin();ii++){
 
		std::cout << *testres <<"  "<<std::endl;
	}

      
      //std::cout<< "results"<<testres<<std::endl;
    }*/
    
    
    
    
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
 
  if (misConnected) {  
  SQLtransaction.clear();
  SQLtransaction.append(request);
  std::cout<< "Transaction:"<< SQLtransaction<<std::endl;
  
  
  res = PQexec(conn, "BEGIN TRANSACTION;");   
    //SQLtransaction.append("COMMIT;");//TODO 
   res = PQexec(conn, SQLtransaction.c_str());
    
    std::cout<< "Dans transaction:"<< SQLtransaction<<std::endl;
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PB COMMIT: %s", PQerrorMessage(conn));
        std::cout<<new std::string(PQerrorMessage(conn));
	PQclear(res);
        disconnect();
	return DATABASE_ERROR;
    }
  
  //res = PQexec(conn, "END;");   
  return 0;
  } else {
    return DATABASE_NOT_CONNECTED; // TODO THROW AN EXCEPTION
  }
 
 
}

/**
 * \brief To reconnect to the database
 * \fn reconnect()
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::reconnect(){
  
  if (!misConnected) {
    
    std::ostringstream out;
    out<<mport;
   
    //TODO test à faire sur le port<0
   
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
  
  return 0;  
  } 
  else {
     // TODO THROW AN EXCEPTION Database already connected
     return DATABASE_NOT_CONNECTED; // TODO THROW AN EXCEPTION
  }
  
  
  
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
      
     //TODO if port < 0 exception
     
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
  
    if (misConnected) {
    // res = PQexec(conn, "BEGIN TRANSACTION;");   
    //SQLtransaction.append("COMMIT;");//TODO 
    //res = PQexec(conn, SQLtransaction.c_str());
    
    //std::cout<< "Dans Commit:"<< SQLtransaction<<std::endl;
    
    /*if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PB COMMIT: %s", PQerrorMessage(conn));
        std::cout<<new std::string(PQerrorMessage(conn));
	PQclear(res);
        disconnect();
	return DATABASE_ERROR;
    }
    //PQclear(res);
 
    std::cout<< "Tout ok dans commit:"<<std::endl;
    
 
    PQclear(res);*/
    
    res = PQexec(conn, "COMMIT TRANSACTION;");
    
    if (!res)
    {
      fprintf(stderr, "PB COMMIT BAS: %s", PQerrorMessage(conn));
        std::cout<<new std::string(PQerrorMessage(conn));
	PQclear(res);
        disconnect();
	return DATABASE_ERROR;
    }
    
    
    return 0;
    } else {
      return DATABASE_NOT_CONNECTED; //TODO throw exception 
    }
  
}

/**
 * \brief To set the db to use
 * \fn virtual int setDb(std::string db)
 * \param db The database to use
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::setDatabase(std::string db){
  
  if (!misConnected) {
    mdatabase = db;
    return 0;
  }
  else {
    return DATABASE_ERROR; //TODO exception database connected
  }
  
  
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
  
    if (misConnected) {
      //res = PQexec(conn, "BEGIN");   
      //res = PQexec(conn, SQLtransaction.c_str());
      
      //PQclear(res);    
      res = PQexec(conn, "ROLLBACK;");
	
	if (!res) {
	  fprintf(stderr, "PB ROLLBACK: %s", PQerrorMessage(conn));
	    std::cout<<new std::string(PQerrorMessage(conn));
	    PQclear(res);
	    disconnect();
	    return DATABASE_ERROR;
	}

      
      
      return 0;
      } else {
	return DATABASE_NOT_CONNECTED; //TODO throw exception 
      }
      
}

