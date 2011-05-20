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

int
MYSQLDatabase::process(string request){
  int reqPos;
  MYSQL* conn = getConnexion(reqPos);
  int res;
  //FIXME remove output
  cout << "@@@@@ PROCESS QUERY: " << request << endl;
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
    releaseConnexion(reqPos);
    throw SystemException(ERRCODE_DBERR, "P-Query error with code "
                                         +convertToString(res)+"{" + request + "}");
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
      if (mysql_field_count(conn) == 0) {
          //FIXME remove output
          cout << mysql_affected_rows(conn) << " rows affected" << endl;
      }
      else {
        // some error occurred
        cout << "Could not retrieve result set" << endl;
        throw SystemException(ERRCODE_DBERR, "P-Query error with code "
                                         +convertToString(res)+"{" + request + "}");
      }
    }
    // more results? -1 = no, >0 = error, 0 = yes (keep looping)
    if ((res = mysql_next_result(conn)) > 0) {
      cout << "Could not execute statement" << endl;
    }
  } while (res == 0);

  releaseConnexion(reqPos);
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
      throw SystemException(ERRCODE_DBERR, "Connection problem with message:"
                            +string(mysql_error(&(mpool[i].mmysql))));
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
 * \brief To set the db to use
 * \fn virtual int setDatabase(string db) = 0
 * \param db The database to use
 * \return 0 on success, an error code otherwise
 */
// int
// MYSQLDatabase::setDatabase(string db){
//   mdatabase = db;
//   for (unsigned int i = 0 ; i<mconfig.getDbPoolSize(); i++) {
//     if (!mysql_select_db(&(mpool[i].mmysql), db.c_str ())) {
//       throw SystemException(ERRCODE_DBERR, "Database problem with message: "+string(mysql_error(&(mpool[i].mmysql))));
//     }
//   }
//   return SUCCESS;
// }
/**
 * \brief To get the result of the latest request (if any result)
 * \fn DatabaseResult* getResult(string request)
 * \return The result of the latest request
 */
DatabaseResult*
MYSQLDatabase::getResult(string request) {
  int reqPos;
  MYSQL* lconn = getConnexion(reqPos);
  MYSQL_RES *res;
  MYSQL_ROW  row;
  MYSQL_FIELD *field;
  vector<vector<string> > results;
  vector<string> attributesNames;
  vector<string> tmp;
  int        size;
  int        i;

  if ((mysql_real_query(lconn, request.c_str (), request.length())) != 0) {
    releaseConnexion(reqPos);
    throw SystemException(ERRCODE_DBERR, "S-Query error with code "+convertToString(res)+"{" + request + "}");
  }
  res = mysql_use_result (lconn);
  size = mysql_num_fields(res);
  if (res==NULL) {
    throw SystemException(ERRCODE_DBERR, "Database problem to get the results with message: "+string(mysql_error(lconn)));
  }
  while ((row=mysql_fetch_row(res))){
    tmp.clear();
    for (i=0;i<size;i++){
      tmp.push_back(string(row[i]));
    }
    results.push_back(tmp);
  }
  while((field = mysql_fetch_field(res))) {
    attributesNames.push_back(string(field->name));
  }
  releaseConnexion(reqPos);

  return new DatabaseResult(results, attributesNames);
}


MYSQL*
MYSQLDatabase::getConnexion(int& id){
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
        return &(mpool[i].mmysql);
      }
    }
    i++;
    // I do not use modulo '%' because i need to be sure i>0
    if (i==mconfig.getDbPoolSize()) {
      i=0;
    }
  }
  throw SystemException(ERRCODE_DBCONN, "Unknown error, cannot get connection on database");
}

void
MYSQLDatabase::releaseConnexion(int pos){
  int ret;
  ret = pthread_mutex_unlock(&(mpool[pos].mmutex));
  if (ret) {
    throw SystemException(ERRCODE_DBCONN, "Failed to release connection lock");
  }
  mpool[pos].mused = false;
}
