/**
 * \file POSTGREDatabase.cpp
 * \brief This file implements a PostGreSQL database.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2011
 */
#include <sstream>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "POSTGREDatabase.hpp"
#include "SystemException.hpp"
#include "utilVishnu.hpp"

using namespace std;

/**
 * \brief Function to process the request in the database
 * \fn    int process(std::string request)
 * \param request The request to process
 * \return raises an exception on error
 */
int
POSTGREDatabase::process(std::string request, int transacId){

  PGresult* res;
  int reqPos;
  PGconn* lconn = getConnection(reqPos);

  if (PQstatus(lconn) == CONNECTION_OK) {
    res = PQexec(lconn, request.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      PQclear(res);
      std::string errorMsg = std::string(PQerrorMessage(lconn));
      errorMsg.append("- Note: The process function must not be used for select request");
      releaseConnection(reqPos);
      throw SystemException(ERRCODE_DBERR, errorMsg);
    }
    PQclear(res);
  }
  else {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBCONN, std::string(PQerrorMessage(lconn)));
  }
  releaseConnection(reqPos);
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
  for (i=0;i<mconfig.getDbPoolSize();i++) {
    if (PQstatus(mpool[i].mconn) != CONNECTION_OK) {
      std::ostringstream out;
      if (mconfig.getDbPort() < 0) {
        throw SystemException(ERRCODE_DBCONN, "The port value is incorrect");
      } else if (mconfig.getDbPort() != 0) {
        out << mconfig.getDbPort();
      }
      // Make a connection to the database
      mpool[i].mconn = PQsetdbLogin(mconfig.getDbHost().c_str(),
                                    out.str().c_str(),
                                    "",
                                    "",
                                    mconfig.getDbName().c_str(),
                                    mconfig.getDbUserName().c_str(),
                                    mconfig.getDbUserPassword().c_str());

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
POSTGREDatabase::POSTGREDatabase(DbConfiguration dbConfig)
 : Database(), mconfig(dbConfig), misConnected(false) {
  int i;
  mpool = new pool_t[mconfig.getDbPoolSize()];
  for (i=0;i<mconfig.getDbPoolSize();i++){
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
  delete [] mpool;
}


/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0
 */
int
POSTGREDatabase::disconnect(){
  int i;
  for (i = 0; i < mconfig.getDbPoolSize(); i++){
    if (mpool[i].mconn != NULL) {
      PQfinish(mpool[i].mconn);
    }
    pthread_mutex_destroy(&(mpool[i].mmutex));
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
POSTGREDatabase::getResult(std::string request, int transacId) {
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> attributesNames;
  std::vector<std::string> tmp;
  int reqPos;
  PGconn* lconn = getConnection(reqPos);

  if (PQstatus(lconn) == CONNECTION_OK) {
    PGresult* res = PQexec(lconn, request.c_str());
    int i;
    int j;

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      PQclear(res);
      releaseConnection(reqPos);
      throw SystemException(ERRCODE_DBERR, std::string(PQerrorMessage(lconn)));
    }
    int nFields = PQnfields(res);
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
    releaseConnection(reqPos);
    PQclear(res);
  } else {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBCONN, "The database is not connected");
  }
  return new DatabaseResult(results, attributesNames);
}

PGconn* POSTGREDatabase::getConnection(int& id){
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
      }
      else {
        mpool[i].mused=true;
        id = i;
        return mpool[i].mconn;
      }
    }
    i++;
    // I do not use modulo '%' because i need to be sure i>0
    if (i==mconfig.getDbPoolSize()) {
      i=0;
    }
  }
  throw SystemException(ERRCODE_DBCONN, "Unknown error, cannot get connexion on database");
}

void POSTGREDatabase::releaseConnection(int pos){
  int ret;
  ret = pthread_mutex_unlock(&(mpool[pos].mmutex));
  if (ret) {
    throw SystemException(ERRCODE_DBCONN, "Fail to release a mutex");
  }
  mpool[pos].mused = false;
}

int
POSTGREDatabase::startTransaction(){
  int reqPos;
  getConnection(reqPos);
  process("BEGIN;", reqPos);
  return reqPos;
}

void
POSTGREDatabase::endTransaction(int transactionID) {
  process("COMMIT;", transactionID);
  releaseConnection(transactionID);
}

void
POSTGREDatabase::cancelTransaction(int transactionID) {
  process("ROLLBACK;", transactionID);
  releaseConnection(transactionID);
}

void
POSTGREDatabase::flush(int transactionID){
  process("COMMIT;", transactionID);
  releaseConnection(transactionID);
}

int
POSTGREDatabase::generateId(string table, string fields, string val, int tid, std::string primary) {
  std::string sqlCommand("INSERT INTO "+table+ fields + " values " +val);
  sqlCommand += ";SELECT currval(pg_get_serial_sequence('"+table+"', '"+primary+"'))";
  vector<string> results = vector<string>();
  vector<string>::iterator iter;
  try{
    boost::scoped_ptr<DatabaseResult> result(getResult(sqlCommand.c_str(), tid));
    if (result->getNbTuples()==0) {
      throw SystemException(ERRCODE_DBERR, "Failure generating the id");
    }
    results.clear();
    results = result->get(0);
    iter = results.begin();
  } catch (SystemException& e){
    throw (e);
  }
  return vishnu::convertToInt(*iter);
}
