/**
 * \file SOCIDatabase.cpp
 * \brief This file implements the SOCI database.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 16/06/12
 */
#include "SOCIDatabase.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "utilVishnu.hpp"

#include <soci/mysql/soci-mysql.h>
#include <soci/postgresql/soci-postgresql.h>


using namespace std;
using namespace vishnu;
using namespace soci;

/**
 * \brief timeout for gettig connection in getConnection
 */
const int CONN_TIMEOUT=15000; // in millisecond


int
SOCIDatabase::process(string request, int transacId){
  int reqPos;
  MYSQL* conn = NULL;
  if (transacId==-1) {
    conn = getConnection(reqPos);
  } else {
    reqPos = -1;
    conn = (&(mpool[transacId].mmysql));
  }

  int res;
  if (request.empty()) {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBERR, "Empty SQL query");
  }
  // The query must always end with a semicolumn when CLIENT_MULTI_STATEMENTS
  // option is set
  if (request.at(request.length()-1) != ';') {
    request.append(";");
  }

  res=mysql_real_query(conn, request.c_str (), request.length());

  if (res) {
// try to reinitialise the socket
    if (mysql_real_connect(&(mpool[reqPos].mmysql),
                           mconfig.getDbHost().c_str(),
                           mconfig.getDbUserName().c_str(),
                           mconfig.getDbUserPassword().c_str(),
                           mconfig.getDbName().c_str(),
                           mconfig.getDbPort(),
                           NULL,
                           CLIENT_MULTI_STATEMENTS) ==NULL) {
      throw SystemException(ERRCODE_DBERR, "Cannot reconnect to the DB"
                            + dbErrorMsg(&(mpool[reqPos].mmysql)));
    }
    res=mysql_real_query(conn, request.c_str (), request.length());
    if (res) {
      // Could not execute the query
      releaseConnection(reqPos);
      throw SystemException(ERRCODE_DBERR, "P-Query error" + dbErrorMsg(conn));
    }
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
        releaseConnection(reqPos);
        throw SystemException(ERRCODE_DBERR, "P-Query error" + dbErrorMsg(conn));
      }
    }
    // more results? -1 = no, >0 = error, 0 = yes (keep looping)
    if ((res = mysql_next_result(conn)) > 0) {
      releaseConnection(reqPos);
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
SOCIDatabase::connect(){
  for (unsigned int i=0; i<mconfig.getDbPoolSize();i++) {

	session & msession = mpool->at(i);
	string connectString="";
	connectString+="dbname="+mconfig.getDbName();
	connectString+=" user="+mconfig.getDbUserName();
	connectString+=" password="+mconfig.getDbUserPassword();
	connectString+=" host="+mconfig.getDbHost();
	connectString+=" port="+mconfig.getDbPort();


	try
	{
		switch(mconfig.getDbType())
		{
			case DbConfiguration::MYSQL :
				msession(mysql,connectString);
				break;
			case DbConfiguration::POSTGRESQL:
				msession(postgresql,connectString);
				break;
			/*case DbConfiguration::ORACLE:
				break;*/
			default:
				throw SystemException
				(ERRCODE_DBERR, "Database instance type unknown or not managed");
				break;
		}
	}
    catch (exception const &e)
    {
        throw SystemException(ERRCODE_DBERR, "Cannot connect to the DB : "+e.what());
    }

  }//for
  return SUCCESS;
}
/**
 * \fn SOCIDatabase(DbConfiguration dbConfig)
 * \brief Constructor, raises an exception on error
 */
SOCIDatabase::SOCIDatabase(DbConfiguration dbConfig)
  : Database(), mconfig(dbConfig), mdbtype(dbConfig.getDbType()) {
  mpool = new connection_pool(mconfig.getDbPoolSize());

  /* for (unsigned int i=0;i<mconfig.getDbPoolSize();i++) {
    pthread_mutex_init(&(mpool[i].mmutex), NULL);
    mpool[i].mused = false;
    mysql_init(&(mpool[i].mmysql));
  }*/
}

/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
SOCIDatabase::~SOCIDatabase(){
  disconnect ();
}
/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int
SOCIDatabase::disconnect(){
  for (unsigned int i = 0 ; i < mconfig.getDbPoolSize() ; i++) {
    (mpool->at(i)).close();
  }
  return SUCCESS;
}


/**
 * \brief To get the result of the latest request (if any result)
 * \param transacId the id of the transaction if one is used
 * \return The result of the latest request
 */
DatabaseResult*
MYSQLDatabase::getResult(string request, int transacId) {
  int reqPos;
  MYSQL* conn = NULL;
  int res;
  if (transacId==-1) {
    conn = getConnection(reqPos);
  } else {
    reqPos = -1;
    conn = (&(mpool[transacId].mmysql));
  }
  // Execute the SQL query
  if ((res=mysql_real_query(conn, request.c_str (), request.length())) != 0) {

// try to reinitialise the socket
    if (mysql_real_connect(&(mpool[reqPos].mmysql),
                           mconfig.getDbHost().c_str(),
                           mconfig.getDbUserName().c_str(),
                           mconfig.getDbUserPassword().c_str(),
                           mconfig.getDbName().c_str(),
                           mconfig.getDbPort(),
                           NULL,
                           CLIENT_MULTI_STATEMENTS) ==NULL) {
      throw SystemException(ERRCODE_DBERR, "Cannot reconnect to the DB"
                            + dbErrorMsg(&(mpool[reqPos].mmysql)));
    }
    res=mysql_real_query(conn, request.c_str (), request.length());
    if (res) {
      releaseConnection(reqPos);
      throw SystemException(ERRCODE_DBERR, "S-Query error" + dbErrorMsg(conn));
    }
  }

  // Get the result handle (does not fetch data from the server)
  MYSQL_RES *result = mysql_use_result(conn);
  if (result == 0) {
    releaseConnection(reqPos);
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


session &
SOCIDatabase::getConnection(int& id){ //TODO:working here

	bool available_connection;
	size_t pos;
	available_connection=mpool->try_lease(pos,CONN_TIMEOUT);

	if(available_connection)
	{
		id = (int) pos; //Transaction ID
		return mpool->at(pos);
	}
	else
	{
		//TIMEOUT PASSED
		id=-1;
		throw SystemException(ERRCODE_DBCONN, "Cannot get available DB connection");
	}

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
SOCIDatabase::startTransaction() {
  int reqPos;
  session & conn = getConnection(reqPos);

  try
  {
	  conn.begin();
  }
  catch (exception const &e)
  {
	  throw SystemException(ERRCODE_DBCONN, "Failed to start transaction");
  }

  // DO NOT RELEASE THE CONNECTION, KEEPING TRANSACTION
  return reqPos;
}

void
MYSQLDatabase::endTransaction(int transactionID) { //TODO: working here
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
SOCIDatabase::cancelTransaction(int transactionID) {

  size_t pos= transactionID;
  session & conn = (mpool->at(pos));

  try
  {
	  conn.rollback();
  }
  catch (exception const &e)
  {
	  mpool->give_back(pos);
	  throw SystemException(ERRCODE_DBCONN, "Failed to start transaction");
  }

  mpool->give_back(pos);
}

void
SOCIDatabase::flush(int transactionID){ //TODO: add try catch ?


  size_t pos= transactionID;
  session & conn = (mpool->at(pos));

  conn.commit();

}

int
MYSQLDatabase::generateId(string table, string fields, string val, int tid) {
  std::string sqlCommand("INSERT INTO "+table+ fields + " values " +val);
  std::string getcpt("SELECT LAST_INSERT_ID() FROM vishnu");
  vector<string> results = vector<string>();
  vector<string>::iterator iter;

  try{
    process(sqlCommand.c_str(), tid);
    boost::scoped_ptr<DatabaseResult> result(getResult(getcpt.c_str(), tid));
    if (result->getNbTuples()==0) {
      throw SystemException(ERRCODE_DBERR, "Failure generating the id");
    }
    results.clear();
    results = result->get(0);
    iter = results.begin();
  } catch (SystemException& e){
    throw (e);
  }
  return convertToInt(*iter);
}

