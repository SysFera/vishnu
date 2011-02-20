#include "OracleDatabase.hpp"

int
OracleDatabase::process(std::string req){
  std::string errorMsg;
  if (mstmt){
    try{
      mstmt->execute();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised executing the query \n");
      InternalVishnuException e(2, errorMsg);
      throw e;
    }
  }else{
    errorMsg.append(" Cannot process an empty statement \n");
    InternalVishnuException e(2, errorMsg);
    throw e;
  }
  return SUCCESS;
}


int 
OracleDatabase::startTransaction(std::string request){
  connect();
}

int 
OracleDatabase::connect(){
  std::string errorMsg;
  try{
    if (!menvironment){
      menvironment = Environment::createEnvironment(oracle::occi::Environment::DEFAULT);
      mcon = menvironment->createConnection(musername, mpwd, mdatabase);
    }
  }catch(oracle::occi::SQLException &e){
    errorMsg.append(" An exception was raised creating the environment and connecting \n");
    InternalVishnuException e(2, errorMsg);
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
  std::string errorMsg;
  if (mcon){
    try{
      mcon->commit();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during commit \n");
      InternalVishnuException e(2, errorMsg);
      throw e;
    }
  }
  return SUCCESS;
}

int
OracleDatabase::rollback(){
  std::string errorMsg;
  if (mcon){
    try {
    mcon->rollback();
    }catch(oracle::occi::SQLException &e){
      errorMsg.append("An exception was raised during the rollback \n");
      InternalVishnuException e(2, errorMsg);
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

  mstmt = mcon->createStatement(request);
  mres = mstmt->executeQuery();
  mres->setCharacterStreamMode(2, 10000);
  std::vector<MetaData> vec = mres->getColumnListMetaData();
  size = vec.size();

  for (i=0;i<size;i++){
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
  return new DatabaseResult(results, attributesNames);
}
