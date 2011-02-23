/**
 * \file OracleDatabase.cpp
 * \brief This file implements an Oracle 11g database.
 * \author Kevin Coulomb
 * \date 31/01/2011
 */
#include "OracleDatabase.hpp"
#include "utilVishnu.hpp"

using namespace std;
using namespace vishnu;

/**
 * \brief Function to process the request in the database
 * \fn    int process(string request)
 * \param request The request to process
 * \return raises an exception on error
 */
int
OracleDatabase::process(string req){
  string errorMsg;
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
 * \fn int startTransaction(string request)
 * \param request The series of requests to process
 * \return raises an exception on error
 */
int
OracleDatabase::startTransaction(string request){
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
  string errorMsg;
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
OracleDatabase::OracleDatabase(string hostname,
                               string username,
                               string pwd,
                               string database,
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
  string errorMsg;
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
  string errorMsg;
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
OracleDatabase::setDatabase(string db){
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
OracleDatabase::getResult(string request) {
  vector<vector<string> > results;
  vector<string> colNames;
  string errorMsg;
  unsigned int nbCol;
  unsigned int i;

  if (mcon == NULL) {
    throw SystemException(ERRCODE_DBCONN, "Database is not connected");
  }
  try {
    mstmt = mcon->createStatement(request);
    mres = mstmt->executeQuery();
    mres->setCharacterStreamMode(2, 10000);
    vector<MetaData> vec = mres->getColumnListMetaData();
    nbCol = vec.size();
    vector<int> colTypes(nbCol);
    colNames.resize(nbCol);

    for (i=0;i<nbCol;i++){
      colTypes[i] = vec[i].getInt(MetaData::ATTR_DATA_TYPE);
      colNames[i] = vec[i].getString(MetaData::ATTR_NAME);
    }
//     unsigned int nbRows = mres->getNumArrayRows();
//     cout << "RESULT CONTAINS " << nbRows << " ROW(s)" << endl;
    int row = 0;
    while(mres->next()){
//     for (int row=0; row < nbRows; row++) {
      cout << "*** ROW " << row++ << endl;
      vector<string> rowValues = vector<string>();
      for (unsigned int i=0 ; i<nbCol; i++){
        cout << i << " : " << colNames[i] << " : ";
        cout << ((colTypes[i] == OCCI_SQLT_NUM) ? "NUM" : "OTHER") << endl;
        unsigned int colId = i + 1; // Oracle count from 1 to size
        string colValue;
        Number num1;
        if (mres->isNull(colId)) {
          cout << "NULL COLUMN" << endl;
          continue;
        }
        if (mres->isTruncated(colId)) {
          cout << "TRUNCATED COLUMN" << endl;
          continue;
        }
        switch(colTypes[i]) {
          case OCCI_SQLT_NUM:
            cout << "Getting number : colId=" << colId << endl;
            num1 = mres->getNumber(colId);
            if (!num1.isNull()) {
//               cout << "NUMBER IS " << ((int) num1) << endl;
//               colValue = num1.toText(menvironment,"9999999");
            } else
              cout << "NULL Number value!" << endl;
            break;
          case OCCI_SQLT_CHR:
            cout << "Getting String : colId=" << colId << endl;
            colValue = mres->getString(colId);
            break;
          default:
            errorMsg.append("Unknown column type (column: ");
            errorMsg.append(colNames[i]);
            errorMsg.append(")");
            throw SystemException(ERRCODE_DBERR, errorMsg);
        }
        cout << "VALUE:" << colValue << endl;
        rowValues.push_back(colValue);
      }
      results.push_back(rowValues);
    }
    mstmt->closeResultSet(mres);
  } catch(oracle::occi::SQLException &e){
    errorMsg.append("SQL Failure in getResult: ");
    errorMsg.append(e.getMessage());
    throw SystemException(ERRCODE_DBERR, errorMsg);
  }

  return new DatabaseResult(results, colNames);
}
