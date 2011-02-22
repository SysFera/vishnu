/**
 * \file OracleDatabase.cpp
 * \brief This file implements an Oracle 11g database.
 * \author Kevin Coulomb
 * \date 31/01/2011
 */
#include "OracleDatabase.hpp"

/**
 * \brief Function to process the request in the database
 * \fn    int process(std::string request)
 * \param request The request to process
 * \return raises an exception on error
 */
int
OracleDatabase::process(std::string req){
  std::string errorMsg;
  if (mstmt){
    try{
      mstmt->execute();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised executing the query \n");
      throw SystemException(ERRCODE_DBERR, errorMsg);
    }
  }else{
    errorMsg.append(" Cannot process an empty statement \n");
    throw SystemException(ERRCODE_DBERR, errorMsg);
  }
  return SUCCESS;
}

/**
 * \brief To start a transaction with the database
 * \fn int startTransaction(std::string request)
 * \param request The series of requests to process
 * \return raises an exception on error
 */
int
OracleDatabase::startTransaction(std::string request){
  connect();
  return SUCCESS;
}

/**
 * \brief To make a connection to the database
 * \fn int connect()
 * \return raises an exception on error
 */
int
OracleDatabase::connect(){
  std::string errorMsg;
  try{
    if (!menvironment){
      menvironment = Environment::createEnvironment(oracle::occi::Environment::DEFAULT);
      mcon = menvironment->createConnection(musername, mpwd, mhost);
    }
  }catch(oracle::occi::SQLException &e){
    errorMsg.append("Exception raised during database connection: ");
    errorMsg.append(e.getMessage());
    throw SystemException(ERRCODE_DBCONN, errorMsg);
  }
  return SUCCESS;
}

/**
 * \fn Database()
 * \brief Constructor
 */
OracleDatabase::OracleDatabase(std::string hostname,
                               std::string username,
                               std::string pwd,
                               std::string database,
                               unsigned int port)
  :Database(), menvironment(NULL), mcon(NULL), mstmt(NULL), mres(NULL),
   mhost(hostname), musername(username), mpwd(pwd), mdatabase(database), mport(port),
   misConnected(false) {
}

/**
 * \fn ~Database()
 * \brief Destructor
 */
OracleDatabase::~OracleDatabase(){
  disconnect();
}

/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0
 */
int
OracleDatabase::disconnect(){
  menvironment->terminateConnection(mcon);
  Environment::terminateEnvironment(menvironment);
  mcon         = NULL;
  menvironment = NULL;
  return SUCCESS;
}

/**
 * \brief To commit a To commit a postgresql transaction transaction
 * \fn int commit()
 * \return raises an exception on error
 */
int
OracleDatabase::commit(){
  std::string errorMsg;
  if (mcon){
    try{
      mcon->commit();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during commit \n");
      throw SystemException(ERRCODE_DBERR, errorMsg);
    }
  } else {
    throw SystemException(ERRCODE_DBCONN, "Database is not connected");
  }
  return SUCCESS;
}


/**
 * \brief To cancel a transaction
 * \fn int rollback()
 * \return raises an exception on error
 */
int
OracleDatabase::rollback(){
  std::string errorMsg;
  if (mcon){
    try {
    mcon->rollback();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during the rollback \n");
      throw SystemException(ERRCODE_DBERR, errorMsg);
    }
  } else {
    throw SystemException(ERRCODE_DBCONN, "Database is not connected");
  }
  return SUCCESS;
}

/**
 * \brief To set the name of the database to use
 * \param db The name of database to use
 * \return raises an exception on error
 */
int
OracleDatabase::setDatabase(std::string db){
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
OracleDatabase::getResult(std::string request) {
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> attributesNames;
  std::vector<std::string> tmp;
  std::string errorMsg;
  int size;
  int i;

  if (mcon == NULL) {
    throw SystemException(ERRCODE_DBCONN, "Database is not connected");
  }
  try {
    mstmt = mcon->createStatement(request);
    mres = mstmt->executeQuery();
    mres->setCharacterStreamMode(2, 10000);
    std::vector<MetaData> vec = mres->getColumnListMetaData();
    size = vec.size();

    for (i=0;i<size;i++){
      // TODO check column type
      attributesNames.push_back(vec[i].getString(vec[i].getAttributeId(i+1)));
    }

    while(mres->next()){
      std::vector<std::string> tmp = std::vector<std::string>();
      for (i=1 ; i<=size; i++){ // Oracle count from 1 to size
        tmp.push_back(mres->getString(i));
      }
      results.push_back(tmp);
    }
    mstmt->closeResultSet(mres);
  } catch(oracle::occi::SQLException &e){
    errorMsg.append("SQL Failure in getResult: ");
    errorMsg.append(e.getMessage());
    throw SystemException(ERRCODE_DBERR, errorMsg);
  }

  return new DatabaseResult(results, attributesNames);
}
