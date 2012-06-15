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
SOCIDatabase::process(string request, int transacId){ //TODO: working here
  int reqPos;
  session& conn;
  size_t pos;

  if (transacId==-1) {
    conn = getConnection(reqPos);
  } else {
    reqPos = -1;
    pos = transacId;
    conn = mpool->at(pos);
  }

  int res;
  if (request.empty()) {
    releaseConnection(reqPos);
    throw SystemException(ERRCODE_DBERR, "Empty SQL query");
  }

  try
  {
      conn<<request;
  }
  catch(exception const &e)
  {
      try
      {
          // try to reconnect the session
          conn.reconnect();
      }
      catch(exception const& e2)
      {
          throw SystemException(ERRCODE_DBERR, "Cannot reconnect to the DB : "
                                      + e2.what());
      }

      try
      {
          conn<<request;
      }
      catch(exception const& e3)
      {
          mpool->give_back(pos);
          throw SystemException(ERRCODE_DBERR, "Cannot reconnect to the DB : "
                                      +e3.what());
      }
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
SOCIDatabase::connect(){

  struct backend_factory & backend;

  switch(mconfig.getDbType())
  {
          case DbConfiguration::MYSQL :
                  backend=mysql;
                  break;
          case DbConfiguration::POSTGRESQL:
                  backend=postgresql;
                  break;
          /*case DbConfiguration::ORACLE:
                  break;*/
          default:
                  throw SystemException
                  (ERRCODE_DBERR, "Database instance type unknown or not managed");
                  break;
  }



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
	    msession.open(backend,connectString);
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
SOCIDatabase::getResult(string request, int transacId) { //TODO:working here
  int reqPos;
  session& conn;
  size_t pos;

  if (transacId==-1) {
    conn = getConnection(reqPos);
  } else {
    reqPos = -1;
    pos = transacId;
    conn = mpool->at(pos);
  }

  // exectue request
  rowset<row> results;
  vector<vector<indicator>> inds;
  try
  {
      conn<<request,into(results,inds);
  }
  catch(exception const &e)
  {
      try
      {
          // try to reconnect the session
          conn.reconnect();
      }
      catch(exception const& e2)
      {
          throw SystemException(ERRCODE_DBERR, "Cannot reconnect to the DB : "
                                      + e2.what());
      }

      try
      {
          conn<<request,into(results);
      }
      catch(exception const& e3)
      {
          mpool->give_back(pos);
          throw SystemException(ERRCODE_DBERR, "Cannot get query results : "
                                      + e3.what());
      }
  }



  vector<string> rowStr;
  vector<vector<string> > resultsStr;


  for(rowset<row>::const_iterator itrs=results.begin(); itrs != results.end(); ++itrs)
    {
      rowStr.clear();
      row const& current_row = *itrs;
      for(size_t j=0; j<current_row.size();j++)
        {
          rowStr.push_back(current_row.get<string>(j));
          //TODO : WARNING! get<string> --> exception bad cast ?
        }

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
SOCIDatabase::releaseConnection(int pos) { //TODO: working here
  if (pos<0){
    return;
  }

  try
  {
      mpool->give_back((size_t)pos);
  }
  catch (exception const &e)
  {
      throw SystemException(ERRCODE_DBCONN, "Cannot release connection lock");
  }

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
SOCIDatabase::endTransaction(int transactionID) {

	size_t pos= transactionID;
	session & conn = (mpool->at(pos));

	try
	{
	  conn.commit();
	}
	catch (exception const &e)
	{
		try
		{
		  conn.rollback();
		  mpool->give_back(pos);
		  throw SystemException(ERRCODE_DBCONN, "Failed to commit the transaction");

		}
		catch (exception const &e)
		{
			mpool->give_back(pos);
			throw SystemException
			(ERRCODE_DBCONN, "Failed to rollback and commit the transaction");


		}
	}

	mpool->give_back(pos);

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

  try
  {
	  conn.commit();
  }
  catch (exception const &e)
  {
	  throw SystemException(ERRCODE_DBCONN, "Failed to commit the transaction");
  }

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




std::string SOCIDatabase::dataToString(const row & r, size_t pos)
{
  std::string dataStr="";
  data_type dt = r.get_properties(pos).get_data_type();
  switch(dt)
  {
    case dt_string:
        dataStr=r.get<std::string>(pos);
        break;
    case dt_double:
        dataStr=convertToString(r.get<double>(pos));
        break;
    case dt_integer:
        dataStr=convertToString(r.get<int>(pos));
        break;
    case dt_unsigned_long:
        dataStr=convertToString(r.get<unsigned long>(pos));
        break;
    case dt_long_long:
        dataStr=convertToString(r.get<long long>(pos));
        break;
    case dt_date:
        std::tm time = r.get<std::tm>(pos);
        dataStr.append(convertToString(time.tm_mon));
        dataStr.append("-");
        dataStr.append(convertToString(time.tm_mday));
        dataStr.append("-");
        dataStr.append(convertToString(time.tm_year));
        dataStr.append("T");
        dataStr.append(convertToString(time.tm_hour));
        dataStr.append(":");
        dataStr.append(convertToString(time.tm_min));
        dataStr.append(":");
        dataStr.append(convertToString(time.tm_sec));
        break;
  }

  return dataStr;
}


vector<string> SOCIDatabase::rowToString(const row & r)
{

  vector<string> rowStr;
  for(size_t i=0;i<r.size();++i)
    {
        string value="";
        //data_type dt = r.get_properties(i).get_data_type();
        indicator ind = r.get_indicator(i);


        switch (ind)
        {
        case i_ok:
            // the data was returned without problems
            value=dataToString(r,i);

            break;
        case i_null:
            // null value
            value="";
            break;
        case i_truncated:
            value="error:truncated";
            break;
        }

        rowStr.push_back(value);

    }

  return rowStr;
}

