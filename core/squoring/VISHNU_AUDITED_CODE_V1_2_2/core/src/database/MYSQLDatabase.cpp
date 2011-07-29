/**
 * \file MYSQLDatabase.cc
 * \brief This file implements the SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */
#include "MYSQLDatabase.hpp"
#include "SystemException.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

string dbErrorMsg(MYSQL *conn) {
  const char *msg = mysql_error(conn);
  return msg != '\0' ? " {" + string(msg) + "}" : "";
}

int
MYSQLDatabase::process(string request){
  int reqPos;
  MYSQL* conn = getConnection(reqPos);
  int res;
  if (request.empty()) {
    throw SystemException(ERRCODE_DBERR, "Empty SQL query");
  }
  // The query must always end with a semicolumn when CLIENT_MULTI_STATEMENTS
  // option is set
  if (request.at(request.length()-1) != ';') {
    request.append(";");
  }

  res=mysql_real_query(conn, request.c_str (), request.length());

  if (res) {
    // Could not execute the query
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBERR, "P-Query error" + dbErrorMsg(conn));
  }
  // Due to CLIENT_MULTI_STATEMENTS option, results must always be retrieved
  // process each statement result
  do {
    // did current statement return data?
    MYSQL_RES *result = mysql_store_result(conn);
    if (result) {
      // yes; process rows and free the result set
      mysql_free_result(result);
    } else {
      // no result set or error
      if (mysql_field_count(conn) != 0) {
        // some error occurred
        throw SystemException(ERRCODE_DBERR, "P-Query error" + dbErrorMsg(conn));
      }
    }
    // more results? -1 = no, >0 = error, 0 = yes (keep looping)
    if ((res = mysql_next_result(conn)) > 0) {
      throw SystemException(ERRCODE_DBERR, "P-Query error" + dbErrorMsg(conn));
    }
  } while (res == 0);

  releaseConnection(reqPos);
  return SUCCESS;
}
/**
 * \brief To make a connection to the database
 * \fn int connect()
 * \return raises an exception on error
 */
int
MYSQLDatabase::connect(){
  for (unsigned int i=0; i<mconfig.getDbPoolSize();i++) {
    if (mysql_real_connect(&(mpool[i].mmysql),
                           mconfig.getDbHost().c_str(),
                           mconfig.getDbUserName().c_str(),
                           mconfig.getDbUserPassword().c_str(),
                           mconfig.getDbName().c_str(),
                           mconfig.getDbPort(),
                           NULL,
                           CLIENT_MULTI_STATEMENTS) ==NULL) {
      throw SystemException(ERRCODE_DBERR, "Cannot connect to the DB"
                            + dbErrorMsg(&(mpool[i].mmysql)));
    }
  }
  return SUCCESS;
}
/**
 * \fn MYSQLDatabase(DbConfiguration dbConfig)
 * \brief Constructor, raises an exception on error
 */
MYSQLDatabase::MYSQLDatabase(DbConfiguration dbConfig)
  : Database(), mconfig(dbConfig) {
  mpool = new pool_t[mconfig.getDbPoolSize()];
  for (unsigned int i=0;i<mconfig.getDbPoolSize();i++) {
    pthread_mutex_init(&(mpool[i].mmutex), NULL);
    mpool[i].mused = false;
    mysql_init(&(mpool[i].mmysql));
  }
}

/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
MYSQLDatabase::~MYSQLDatabase(){
  disconnect ();
}
/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int
MYSQLDatabase::disconnect(){
  for (unsigned int i = 0 ; i < mconfig.getDbPoolSize() ; i++) {
    mysql_close (&(mpool[i].mmysql));
  }
  return SUCCESS;
}


/**
 * \brief To get the result of the latest request (if any result)
 * \fn DatabaseResult* getResult(string request)
 * \return The result of the latest request
 */
DatabaseResult*
MYSQLDatabase::getResult(string request) {
  int reqPos;
  MYSQL* conn = getConnection(reqPos);
  // Execute the SQL query
  if ((mysql_real_query(conn, request.c_str (), request.length())) != 0) {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBERR, "S-Query error" + dbErrorMsg(conn));
  }
  // Get the result handle (does not fetch data from the server)
  MYSQL_RES *result = mysql_use_result(conn);
  if (result == 0) {
    throw SystemException(ERRCODE_DBERR, "Cannot get query results" + dbErrorMsg(conn));
  }
  int size = mysql_num_fields(result);
  // Fetch data rows
  MYSQL_ROW row;
  vector<string> rowStr;
  vector<vector<string> > results;
  while ((row = mysql_fetch_row(result))) {
    rowStr.clear();
    for (unsigned int i=0;i<size;i++){
      rowStr.push_back(string(row[i] ? row[i] : ""));
    }
    results.push_back(rowStr);
  }
  // Fetch column names
  MYSQL_FIELD *field;
  vector<string> attributesNames;
  while((field = mysql_fetch_field( result))) {
    attributesNames.push_back(string(field->name));
  }
  // Finalize
  releaseConnection(reqPos);
  return new DatabaseResult(results, attributesNames);
}


MYSQL*
MYSQLDatabase::getConnection(int& id){
  int i = 0;
  int locked;
  // Looking for an unused connection (will block until a connection is free)
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
        return &(mpool[i].mmysql);
      }
    }
    i++;
    // I do not use modulo '%' because i need to be sure i>0
    if (i==mconfig.getDbPoolSize()) {
      i=0;
    }
  }
  throw SystemException(ERRCODE_DBCONN, "Cannot get available DB connection");
}

void
MYSQLDatabase::releaseConnection(int pos) {
  int ret = pthread_mutex_unlock(&(mpool[pos].mmutex));
  if (ret) {
    throw SystemException(ERRCODE_DBCONN, "Cannot release connection lock");
  }
  mpool[pos].mused = false;
}
