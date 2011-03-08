/**
* \file MonitorUMS.cpp
* \brief This file presents the implementation of the UMS Monitor
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001
*/

#include "MonitorUMS.hpp"

/**
* \brief Constructor, raises an exception on error
* \fn ServerUMS(std::string cfg)
* \param interval The interval to check the database
*/
MonitorUMS::MonitorUMS(int interval) {
minterval = interval;
mdatabaseVishnu = NULL;
}

/**
* \brief Destructor
* \fn ~MonitorUMS()
*/

MonitorUMS::~MonitorUMS() {
}

/**
* \brief To initialize the UMS monitor with individual parameters instead of configuration file
* \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword)
* \param vishnuId The password of the root user vishnu_user for the connection with the database
* \param dbType   The type of the database (POSTGREDB|ORACLEDB)
* \param dbHost   The host of the database server
* \param dbUsername The name of the database user on the server
* \param dbPassword The password of the database user on the server
* \return raises an execption
*/
void
MonitorUMS::init(int vishnuId,
                int dbType,
                std::string dbHost,
                std::string dbUsername,
                std::string dbPassword) {

  DbFactory factory;

  //To get database instance
  mdatabaseVishnu = factory.getDatabaseInstance(dbType,
                                                dbHost,
                                                dbUsername,
                                                dbPassword,
                                                DATABASENAME);

  DatabaseResult *result;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(vishnuId));


  try {
    /*connection to the database*/
    mdatabaseVishnu->connect();

    /* Checking of vishnuid on the database */
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
    std::cout.flush();
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;

    }

  } catch (VishnuException& e) {
        exit(0);
  }

}

/**
* \brief To launch the UMS Monitor
* \fn int run()
* \return raises an exception
*/
int
MonitorUMS::run() {

  std::vector<std::string>::iterator ii;
  std::vector<std::string> tmp;
  SessionServer closer;

  try {
    DatabaseResult *result = closer.getSessionToclosebyTimeout();

    if (result->getNbTuples() != 0) {
      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        tmp.clear();
        tmp = result->get(i);

        ii=tmp.begin();
        SessionServer sessionServer (*ii);

        try {
          //closure of the session
          sessionServer.close();
        }
        catch (VishnuException& e) {

          string errorInfo =  e.buildExceptionString();
        }
      }
    }
    sleep(minterval);

  } catch (VishnuException& e) {
    string errorInfo =  e.buildExceptionString();
  }
  return 0;
}
