/**
 * \file MYSQLDatabase.cc
 * \brief This file implements the SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */
#include "MYSQLDatabase.hh"

int 
MYSQLDatabase::process(std::string request){
  if (misConnected)
    return mysql_real_query(&mmysql, request.c_str (), request.length());
  return DATABASENOTCONNECTED;
}
  /**
   * \brief To reconnect to the database
   * \fn reconnect()
   * \return 0 on success, an error code otherwise
   */
int 
MYSQLDatabase::reconnect(){
  return 0;
}
  /**
   * \fn Database(std::string hostname,
   *		  std::string username,
   *		  std::string pwd,
   *		  std::string database = "",
   *		  unsigned int port = 0)
   * \brief Constructor, raises an exception on error
   */
MYSQLDatabase::MYSQLDatabase(std::string hostname,
		      std::string username,
		      std::string pwd,
		      std::string database,
		      unsigned int port){

  mhost        = hostname;
  musername    = username;
  mpwd         = pwd;
  mdatabase    = database;
  misConnected = false;
  mport        = port;  
  mysql_init(&mmysql);
  if (!mysql_real_connect(&mmysql, mhost.c_str(), musername.c_str(), mpwd.c_str(),
			  mdatabase.c_str (), mport, NULL, 0)){
    fprintf(stderr, "Fail to connect to the database because : %s\n",
	    mysql_error(&mmysql));
    // TODO THROW AN EXCEPTION
    return;
  }
  misConnected = true;
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
  if (!misConnected)
    return DATABASENOTCONNECTED;
  mysql_close (&mmysql);
  return 0;
}


/**
 * \brief To commit a mysql transaction
 * \fn int commit()
 * \return 0 on success, an error code otherwise
 */
int
MYSQLDatabase::commit (){
  if (!misConnected)
    return DATABASENOTCONNECTED;
  return mysql_commit(&mmysql);
}
/**
 * \brief To set the db to use
 * \fn virtual int setDb(std::string db) = 0
 * \param db The database to use
 * \return 0 on success, an error code otherwise
 */
int
MYSQLDatabase::setDatabase(std::string db){
  if (!misConnected)
    return DATABASENOTCONNECTED;
  return mysql_select_db(&mmysql, db.c_str ());
}
/**
 * \brief To get the result of the latest request (if any result)
 * \fn void* getResult()
 * \return The result of the latest request
 */
std::vector<std::string>
MYSQLDatabase::getResult(){
  if (!misConnected)
    return std::vector<std::string>();
  MYSQL_RES *res;
  MYSQL_ROW  row;
  int        size = mysql_num_fields(res);
  int        i;
  std::string tmp;
  mres.clear();
  res = mysql_use_result (&mmysql);
  while ((row=mysql_fetch_row(res))){
    tmp.clear();
    for (i=0;i<size;i++){
      tmp += std::string(row[i]);
    }
    mres.push_back(tmp);
  }
  return mres;
}
/**
 * \brief To cancel a transaction
 * \fn int rollback()
 * \return 0 on success, an error code otherwise
 */
int
MYSQLDatabase::rollback(){
  if (misConnected)
    return mysql_rollback (&mmysql);
  return DATABASENOTCONNECTED;
}
