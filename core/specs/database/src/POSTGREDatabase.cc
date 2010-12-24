/**
 * \file POSTGREDatabase.cc
 * \brief This file implements a PostGreSQL database.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 15/12/10
 */

#include "POSTGREDatabase.hh"

/**
 * \brief Function to process the request in the database
 * \fn    int createDatabase()
 * \param request The request to process
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::process(std::string request){
  return 0;
}

/**
 * \brief To reconnect to the database
 * \fn reconnect()
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::reconnect(){
  return 0;
}

/**
 * \fn Database()
 * \brief Constructor, raises an exception on error
 */
POSTGREDatabase::POSTGREDatabase(std::string hostname,
		  std::string username,
		  std::string pwd,
		  std::string database,
		  unsigned int port){}
/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
POSTGREDatabase::~POSTGREDatabase(){}
/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int 
POSTGREDatabase::disconnect(){
  return 0;
}

/**
 * \brief To commit a mysql transaction
 * \fn virtual int commit()
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::commit (){
  return 0;
}

/**
 * \brief To set the db to use
 * \fn virtual int setDb(std::string db)
 * \param db The database to use
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::setDatabase(std::string db){
  return 0;
}

/**
 * \brief To get the result of the latest request (if any result)
 * \fn std::vector<std::string> getResult()
 * \return The result of the latest request
 */
std::vector<std::string>
POSTGREDatabase::getResult(){
  return std::vector<std::string>();
}

/**
 * \brief To cancel a transaction
 * \fn int rollback()
 * \return 0 on success, an error code otherwise
 */
int
POSTGREDatabase::rollback(){
  return 0;
}

