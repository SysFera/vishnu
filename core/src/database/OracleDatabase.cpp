#include "OracleDatabase.hpp"

int
OracleDatabase::process(){
  if (mstmp){
    try{
      mstmt->execute();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised executing the query \n");
      SystemException e(2, errorMsg);
      throw e;
    }
  }else{
    errorMsg.append(" Cannot process an empty statement \n");
    SystemException e(2, errorMsg);
    throw e;
  }
  return SUCCESS;
}


int OracleDatabase::startTransaction(string request){
  connect();
}

int OracleDatabase::connect(){
  try{
    if (!menvironment){
      menvironment = Environment::createEnvironment(oracle::occi::Environment::DEFAULT);
      mcon = menvironment->createConnection(musername, mpwd, mdatabase);
    }
  }catch(oracle::occi::SQLException &e){
    errorMsg.append(" An exception was raised creating the environment and connecting \n");
    SystemException e(2, errorMsg);
    throw e;
  }
  return SUCCESS;
}

OracleDatabase::OracleDatabase(std::string hostname,
			       std::string username,
			       std::string pwd,
			       std::string database,
			       unsigned int port):Database(){
  if (port < 0) {
    SystemException e(2, "The port value is incorrect");
    throw e; 
  }

  mhost        = hostname;
  musername    = username;
  mpwd         = pwd;
  mdatabase    = database;
  misConnected = false;
  mport        = port;     
  mcon         = NULL;
  menvironment = NULL;
  mres         = NULL;
  mstmt        = NULL;
}


OracleDatabase::~OracleDatabase(){
  disconnect();
}


int 
OracleDatabase::disconnect(){
  menvironment->terminateConnection(mcon);
  mcon         = NULL;
  menvironment = NULL;
  return SUCCESS;
}

int
OracleDatabase::commit(){
  if (mcon){
    try{
      mcon->commit();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during commit \n");
      SystemException e(2, errorMsg);
      throw e;
    }
  }
  return SUCCESS;
}

int
OracleDatabase::rollback(){
  if (mcon){
    try {
    mcon->rollback();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during the rollback \n");
      SystemException e(2, errorMsg);
      throw e;
    }
  }
  return SUCCESS;
}

int
OracleDatabase::setDatabase(std::string db){
  mdatabase = db;
  return SUCCESS;
}


DatabaseResult*
OracleDatabase::getResult(std::string request) {
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> attributesNames;
  std::vector<std::string> tmp;
  int size;
  int i;
  stmt = con->createStatement(request);
  res = stmt->execute();
  res->setCharacterStreamMode(2, 10000);
  vector<MetaData> vec = res->getColumnListMetaData();
  size = vec.size();

  while(res->next()){
    std::vector<std::string> tmp = std::vector<std::string>();
    for (i=1 ; i<=size; i++){ // Oracle count from 1 to size
      tmp.push_back(reg->getString(i));
    }
    results.push_back(tmp);
  }
  stmt->closeResultSet(res);
  return new DatabaseResult(results, attributesNames);
}
