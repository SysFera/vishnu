/**
 * \file MYSQLDatabase.cpp
 * \brief This file implements the SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include "SystemException.hpp"
#include "Logger.hpp"
#include "utilVishnu.hpp"
#include "errmsg.h"
#include "MYSQLDatabase.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <vector>
#include <mysqld_error.h>


inline std::string dbErrorMsg(MYSQL *conn)
{
  return std::string( mysql_error(conn) );
}

/**
 * \brief Function to process the request in the database
 * \param query The SQL query to process
 * \param transacId the id of the transaction if one is used
 * \return A pair<int, int> containing the return code and LAST_INSERT_ID (only useful for insertion). Raises an exception on error
 */
std::pair<int, uint64_t>
MYSQLDatabase::process(const std::string& query, int transacId)
{

  std::pair<MYSQL*, int>
      connectionInfo = getConnectionFromPool(transacId);

  if (query.empty()) {
    releaseConnection(connectionInfo.second);
    throw SystemException(ERRCODE_DBERR, "Empty SQL query");
  }

  std::string preparedQuery = query;
  if (preparedQuery.at(preparedQuery.length()-1) != ';') {
    preparedQuery.append(";"); // query must end with semicolom ';'
  }

  int rc = mysql_real_query(connectionInfo.first, preparedQuery.c_str(), preparedQuery.length());
  if (rc != 0) {
    raiseOnCriticalMysqlError(connectionInfo.first, connectionInfo.second);

    connectPoolIndex(connectionInfo.second);  // try to reinitialise the socket
    rc = mysql_real_query(connectionInfo.first, preparedQuery.c_str (), preparedQuery.length());

    raiseExceptionIfMysqlError(rc, connectionInfo.first, connectionInfo.second);
  }

  // Due to CLIENT_MULTI_STATEMENTS option, results must always be retrieved process each statement result
  do {
    MYSQL_RES* mysqlResultPtr = mysql_store_result(connectionInfo.first);
    if (mysqlResultPtr) { // did current statement return data?
      mysql_free_result(mysqlResultPtr);
    } else {
      if (mysql_field_count(connectionInfo.first) != 0) { // some error occurred
        releaseConnection(connectionInfo.second);
        throw SystemException(ERRCODE_DBERR, dbErrorMsg(connectionInfo.first));
      }
    }
    // more results? -1 = no, >0 = error, 0 = yes (keep looping)
    rc = mysql_next_result(connectionInfo.first);
    if (rc > 0) {
      raiseExceptionIfMysqlError(rc, connectionInfo.first, connectionInfo.second);
    }
  } while (rc == 0);

  uint64_t lastId = mysql_insert_id(connectionInfo.first);
  releaseConnection(connectionInfo.second);

  return std::pair<int, uint64_t>(0, lastId);
}
/**
 * \brief To make a connection to the database
 * \return raises an exception on error
 */
int
MYSQLDatabase::connect(){
  for (unsigned int i=0; i<mconfig.getDbPoolSize();i++) {
    connectPoolIndex(i);
  }
  return 0;
}

/**
* \brief To make a connection to the database and store the return db handler to a given pool's index
* \param poolIdx the index in connexion pool
* \return raises an exception on error
*/
void
MYSQLDatabase::connectPoolIndex(const int& poolIdx) {

  int ssl_flag = 0;

  if (mconfig.getUseSsl()) {
    ssl_flag = CLIENT_SSL;
    mysql_ssl_set(&(mpool[poolIdx].mmysql),
                  NULL,
                  NULL,
                  (mconfig.getSslCaFile().empty()? NULL : mconfig.getSslCaFile().c_str()),
                  NULL,
                  NULL);
  }
  if (mysql_real_connect(&(mpool[poolIdx].mmysql),
                         mconfig.getDbHost().c_str(),
                         mconfig.getDbUserName().c_str(),
                         mconfig.getDbUserPassword().c_str(),
                         mconfig.getDbName().c_str(),
                         mconfig.getDbPort(),
                         NULL,
                         CLIENT_MULTI_STATEMENTS|ssl_flag) ==NULL) {
    throw SystemException(ERRCODE_DBERR, "Cannot connect to the DB" + dbErrorMsg(&(mpool[poolIdx].mmysql)));
  }
  mysql_set_character_set(&(mpool[poolIdx].mmysql), "utf8");
}

/**
 * \brief Constructor, raises an exception on error
 */
MYSQLDatabase::MYSQLDatabase(DbConfiguration dbConfig)
  : Database(), mconfig(dbConfig) {
  mysql_library_init(0, NULL, NULL);
  mpool = new pool_t[mconfig.getDbPoolSize()];
  for (unsigned int i=0;i<mconfig.getDbPoolSize();i++) {
    pthread_mutex_init(&(mpool[i].mmutex), NULL);
    mpool[i].mused = false;
    mysql_init(&(mpool[i].mmysql));
  }
}

/**
 * \brief Destructor, raises an exception on error
 */
MYSQLDatabase::~MYSQLDatabase(){
  disconnect();
  mysql_library_end();
  delete [] mpool;
}
/**
 * \brief To disconnect from the database
 * \return 0 on success, an error code otherwise
 */
int
MYSQLDatabase::disconnect(){
  for (unsigned int i = 0 ; i < mconfig.getDbPoolSize() ; i++) {
    mysql_close (&(mpool[i].mmysql));
  }
  return 0;
}


/**
* \brief To get the result of a select request
* \param query The request to process
* \param transacId the id of the transaction if one is used
* \return An object which encapsulates the database results
*/
DatabaseResult*
MYSQLDatabase::getResult(const std::string& query, int transacId) {

  // get connection info
  std::pair<MYSQL*, int>
      connectionInfo  = getConnectionFromPool(transacId);

  int rc = mysql_real_query(connectionInfo.first, query.c_str (), query.length());
  if (rc != 0) {
    raiseOnCriticalMysqlError(connectionInfo.first, connectionInfo.second);
    connectPoolIndex(connectionInfo.second);  // try to reinitialise the socket
    rc = mysql_real_query(connectionInfo.first, query.c_str (), query.length());
    raiseExceptionIfMysqlError(rc, connectionInfo.first, connectionInfo.second);
  }

  // check for error
  MYSQL_RES* mysqlResultPtr = mysql_use_result(connectionInfo.first);
  if (! mysqlResultPtr) {
    raiseExceptionIfMysqlError(rc, connectionInfo.first, connectionInfo.second);
  }

  // parse result
  int size = mysql_num_fields(mysqlResultPtr);

  // Fetch rows data
  MYSQL_ROW row;
  std::vector<std::string> rowStr;
  std::vector<std::vector<std::string> > results;
  while ((row = mysql_fetch_row(mysqlResultPtr))) {
    rowStr.clear();
    for (unsigned int i=0;i<size;i++){
      rowStr.push_back(std::string(row[i] ? row[i] : ""));
    }
    results.push_back(rowStr);
  }

  // Fetch column names
  MYSQL_FIELD *field;
  std::vector<std::string> attributesNames;
  while((field = mysql_fetch_field( mysqlResultPtr))) {
    attributesNames.push_back(std::string(field->name));
  }

  // Free the result
  mysql_free_result(mysqlResultPtr);

  // Finalize
  releaseConnection(connectionInfo.second);
  return new DatabaseResult(results, attributesNames);
}


MYSQL*
MYSQLDatabase::getConnection(int& id){
  int poolIndex = 0;
  int locked;
  // Looking for an unused connection (will block until a connection is free)
  while (true) {
    // If the connection is not used
    if (!mpool[poolIndex].mused) {
      locked = pthread_mutex_trylock(&(mpool[poolIndex].mmutex));
      // If lock fails-> the mutex was taken before trylock call
      if (locked) {
        // Try next connexion
        sleep(2);
        continue;
      } else {
        mpool[poolIndex].mused=true;
        id = poolIndex;
        return &(mpool[poolIndex].mmysql);
      }
    }
    ++poolIndex;
    // I do not use modulo '%' because i need to be sure i>0
    if (poolIndex==mconfig.getDbPoolSize()) {
      poolIndex = 0;
      sleep(10);
    }
  }
  throw SystemException(ERRCODE_DBCONN, "Cannot get available DB connection");
}

void
MYSQLDatabase::releaseConnection(int pos) {
  if (pos==-1){
    return;
  }
  int ret = pthread_mutex_unlock(&(mpool[pos].mmutex));
  if (ret) {
    throw SystemException(ERRCODE_DBCONN, "Cannot release connection lock");
  }
  mpool[pos].mused = false;
}

int
MYSQLDatabase::startTransaction() {
  int reqPos;
  MYSQL* conn = getConnection(reqPos);
  bool ret = mysql_autocommit(conn, false);
  if (ret) {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBCONN, "Failed to start transaction");
  }
  // DO NOT RELEASE THE CONNECTION, KEEPING TRANSACTION
  return reqPos;
}

void
MYSQLDatabase::endTransaction(int transactionID) {
  bool ret;
  MYSQL* conn = (&(mpool[transactionID].mmysql));
  ret = mysql_commit(conn);
  if (ret) {
    ret = mysql_rollback(conn);
    if (ret) {
      releaseConnection(transactionID);
      throw SystemException(ERRCODE_DBCONN, "Failed to rollback and commit the transaction");
    }
    releaseConnection(transactionID);
    throw SystemException(ERRCODE_DBCONN, "Failed to commit the transaction");
  }
  ret = mysql_autocommit(conn, true);
  if (ret) {
    ret = mysql_rollback(conn);
    if (ret) {
      releaseConnection(transactionID);
      throw SystemException(ERRCODE_DBCONN, "Failed to rollback and end the transaction");
    }
    releaseConnection(transactionID);
    throw SystemException(ERRCODE_DBCONN, "Failed to end the transaction");
  }
  releaseConnection(transactionID);
}

void
MYSQLDatabase::cancelTransaction(int transactionID) {
  bool ret;
  MYSQL* conn = (&(mpool[transactionID].mmysql));
  ret = mysql_rollback(conn);
  if (ret) {
    releaseConnection(transactionID);
    ret = mysql_autocommit(conn, true);
    throw SystemException(ERRCODE_DBCONN, "Failed to cancel the transaction");
  }
  ret = mysql_autocommit(conn, true);
  releaseConnection(transactionID);
}

void
MYSQLDatabase::flush(int transactionID){
  MYSQL* conn = (&(mpool[transactionID].mmysql));
  mysql_commit(conn);
}

/**
 * @brief escapeData : transform a sql data to a SQL-escaped string
 * @param data: the string to transform
 * @return a espaced string
 */
std::string
MYSQLDatabase::escapeData(const std::string& data)
{
  size_t len = data.size();
  char escapedSql[2*len + 1];

  int connId;
  MYSQL* conn = getConnection(connId);
  size_t escapedSqlLen = mysql_real_escape_string(conn, escapedSql, data.c_str(), len);
  releaseConnection(connId);

  return std::string(escapedSql, escapedSqlLen);
}

/**
 * @brief Return the last inserted id
 * @param transactionId The transaction id
 * @param errorMsg OUT old error message in case of error
 * @return the id or -1 in case of error.
 */
int
MYSQLDatabase::lastInsertedId(int transactionId, std::string& errorMsg)
{
  std::string query = boost::str(boost::format("SELECT LAST_INSERT_ID()"));
  int lastId = -1;
  try{
    boost::scoped_ptr<DatabaseResult> dbResult(getResult(query, transactionId));
    if (dbResult->getNbTuples() != 0) {
      std::vector<std::string> resultList = dbResult->get(0);
      lastId = vishnu::convertToInt(*(resultList.begin()));
    } else {
      errorMsg = boost::str(boost::format("%1% return %2% tuples") % query % dbResult->getNbTuples());
    }
  } catch (VishnuException& ex){
    errorMsg = std::string(ex.what());
  }
  if (lastId < 0) {
    LOG(boost::str(boost::format("[ERROR] %1%") % errorMsg), LogErr);
  }
  return lastId;
}


/**
 * @brief Get a connection from the pool of connections
 * @param transaction The id of the related transaction
 * @return Pair of connection and index in the pool
 */
std::pair<MYSQL*, int>
MYSQLDatabase::getConnectionFromPool(int transaction)
{
  std::pair<MYSQL*, int> result;
  result.first = NULL;
  if (transaction==-1) {
    result.first = getConnection(result.second);
  } else {
    result.second = -1;
    result.first = (&(mpool[transaction].mmysql));
  }
  return result;
}


/**
 * @brief Throw exception if the latest query exit with fatal error
 * @param conn The MYSQL connection pointer
 * @param poolIndex The index of the connection in the pool
 */
void
MYSQLDatabase::raiseOnCriticalMysqlError(MYSQL* conn, int poolIndex)
{
  int ecode = mysql_errno(conn);
  if (ecode != CR_SERVER_LOST && ecode != CR_SERVER_GONE_ERROR) {
    releaseConnection(poolIndex);
    throw SystemException(ERRCODE_DBERR, dbErrorMsg(conn));
  }
}

/**
 * @brief Raise exception if a MySQL call exit with non-zero error code
 * @param rc MySQL api return code
 * @param conn The MYSQL connection pointer
 * @param poolIndex The index of the connection in the pool
 */
void
MYSQLDatabase::raiseExceptionIfMysqlError(int rc, MYSQL* conn, int poolIndex)
{
  if (rc != 0) {
    releaseConnection(poolIndex);
    throw SystemException(ERRCODE_DBERR, dbErrorMsg(conn));
  }
}


