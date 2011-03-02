/**
 * \file POSTGREDatabase.cpp
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
  int reqPos;
  PGconn* lconn = getConnexion(reqPos);
  
  if (PQstatus(lconn) == CONNECTION_OK) {
    res = PQexec(lconn, request.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      PQclear(res);
      std::string errorMsg = std::string(PQerrorMessage(lconn));
      errorMsg.append("- Note: The process function must not be used for select request");
      releaseConnexion(reqPos);
      throw SystemException(ERRCODE_DBERR, errorMsg);
    }
    PQclear(res);
  }
  else {
    releaseConnexion(reqPos);
    throw SystemException(ERRCODE_DBCONN, std::string(PQerrorMessage(lconn)));
  }
  releaseConnexion(reqPos);
  return SUCCESS;
}


/**
 * \brief To make a connection to the database
 * \fn int connect()
 * \return raises an exception on error
 */
int
POSTGREDatabase::connect(){
  int i;
  for (i=0;i<POOLSIZE;i++) {
    if (PQstatus(mpool[i].mconn) != CONNECTION_OK) {
      std::ostringstream out;
      out << mport;
      if (mport < 0) {
	throw SystemException(ERRCODE_DBCONN, "The port value is incorrect");
      }
      
      // Make a connection to the database
      mpool[i].mconn = PQsetdbLogin(mhost.c_str(), "", "",
			   out.str().c_str(),
			   mdatabase.c_str(),
			   musername.c_str(),
			   mpwd.c_str());
      
      if (PQstatus(mpool[i].mconn) != CONNECTION_OK) {
	throw SystemException(ERRCODE_DBCONN, std::string(PQerrorMessage(mpool[i].mconn)));
      }
      misConnected = true;
    }
    else {
      throw SystemException(ERRCODE_DBCONN, "The database is already connected");
    }
  }
  return SUCCESS;
}

/**
 * \fn Database()
 * \brief Constructor
 */
POSTGREDatabase::POSTGREDatabase(std::string hostname,
                                 std::string username,
                                 std::string pwd,
                                 std::string database,
                                 unsigned int port)
 : Database(), mhost(hostname), musername(username), mpwd(pwd),
    mdatabase(database), mport(port), misConnected(false), mSQLtransaction() {
  int i;
  for (i=0;i<POOLSIZE;i++){
    pthread_mutex_init(&(mpool[i].mmutex), NULL);
    mpool[i].mused = false;
    mpool[i].mconn = NULL;
  }
}

/**
 * \fn ~Database()
 * \brief Destructor
 */
POSTGREDatabase::~POSTGREDatabase(){
  disconnect();
}


/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0
 */
int
POSTGREDatabase::disconnect(){
  int i;
  for (i=0;i<25;i++){
    if (mpool[i].mconn != NULL) {
      PQfinish(mpool[i].mconn);
    }
    pthread_mutex_destroy(&(mpool[i].mmutex));
  }
  return SUCCESS;
}

/**
 * \brief To set the name of the database to use
 * \param db The name of database to use
 * \return raises an exception on error
 */
int
POSTGREDatabase::setDatabase(std::string db){

  mdatabase = db;
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
  int reqPos;
  PGconn* lconn = getConnexion(reqPos);

  if (PQstatus(lconn) == CONNECTION_OK) {
    res = PQexec(lconn, request.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      PQclear(res);
      releaseConnexion(reqPos);
      throw SystemException(ERRCODE_DBERR, std::string(PQerrorMessage(lconn)));
    }
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++) {
      attributesNames.push_back(std::string(PQfname(res, i)));
    }

    for (i = 0; i < PQntuples(res); i++) {
      tmp.clear();
      for (j = 0; j < nFields; j++) {
        tmp.push_back(std::string(PQgetvalue(res, i, j)));
      }
      results.push_back(tmp);
    }
    releaseConnexion(reqPos);
    return new DatabaseResult(results, attributesNames);
  }
  else {
    releaseConnexion(reqPos);
    throw SystemException(ERRCODE_DBCONN, "The database is not connected");
  }
  releaseConnexion(reqPos);
  return SUCCESS;
}

PGconn* POSTGREDatabase::getConnexion(int& id){
  int i = 0;
  int locked;
  // Looking for an unused connection
  while (true) {
    // If the connection is not used
    if(!mpool[i].mused) {
      locked = pthread_mutex_trylock(&(mpool[i].mmutex));
	// If lock fails-> the mutex was taken before trylock call
      if (locked) {
	// Try next connexion
	continue;
      }else {
	mpool[i].mused=true;
	id = i;
	return mpool[i].mconn;
      }
    }
    i++;
    // I do not use modulo '%' because i need to be sure i>0
    if (i==POOLSIZE) {
      i=0;
    }
  }
  throw SystemException(ERRCODE_DBCONN, "Unknown error, cannot get connexion on database");
}

void POSTGREDatabase::releaseConnexion(int pos){
  int ret;
  ret = pthread_mutex_unlock(&(mpool[pos].mmutex));
  if (ret) {
    throw SystemException(ERRCODE_DBCONN, "Fail to release a mutex");
  }
  mpool[pos].mused = false;
}
