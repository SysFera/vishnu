/**
 * \file MockDatabase.cpp
 * \brief This file implements the SQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */
#include "MockDatabase.hpp"

std::pair<int, uint64_t>
MockDatabase::process(std::string request, int transacId){
  return std::pair<int, uint64_t>(SUCCESS, 0);
}
/**
 * \brief To make a connection to the database
 * \fn int connect()
 * \return raises an exception on error
 */
int
MockDatabase::connect(){
  return SUCCESS;
}
/**
 * \fn MockDatabase(DbConfiguration dbConfig)
 * \brief Constructor, raises an exception on error
 */
MockDatabase::MockDatabase(DbConfiguration dbConfig)
: Database() {
}

/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
MockDatabase::~MockDatabase(){
}
/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int
MockDatabase::disconnect(){
  return SUCCESS;
}


/**
 * \brief To get the result of the latest request (if any result)
 * \param transacId the id of the transaction if one is used
 * \return The result of the latest request
 */
DatabaseResult*
MockDatabase::getResult(std::string request, int transacId) {
  std::vector<std::vector<std::string> > result;
  std::vector<std::string> param;
  return new DatabaseResult(result, param);
}

int
MockDatabase::startTransaction() {
  return 1;
}

void
MockDatabase::endTransaction(int transactionID) {
}

void
MockDatabase::cancelTransaction(int transactionID) {
}

void
MockDatabase::flush(int transactionID){
}

int
MockDatabase::generateId(std::string table, std::string fields, std::string val, int tid, std::string primary) {
  return 1;
}


std::string
MockDatabase::escapeData(const std::string& data) {
  return data;
}

