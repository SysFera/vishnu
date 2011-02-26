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
std::cout << "INTERVAL:" << interval << endl;;
minterval = interval;
mdatabaseVishnu = NULL;
}

/**
* \brief Destructor
* \fn ~MonitorUMS()
*/

MonitorUMS::~MonitorUMS() {
}

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

  Vishnuid::mvishnuid = convertToString(vishnuId);
  DatabaseResult *result;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+Vishnuid::mvishnuid);
  std::cout << "SQL COMMAND:" << sqlCommand << std::endl;


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
        std::cout << "Error: " << e.getMsg() << std::endl;
        std::cout << e.what() << std::endl;
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

  //DatabaseResult *result;
  std::vector<std::string>::iterator ii;
  std::vector<std::string> tmp;

  std::string sqlClosure;
  std::string sqlCommand("SELECT sessionkey from vsession where \
  EXTRACT( epoch FROM  CURRENT_TIMESTAMP ) - EXTRACT( epoch FROM lastconnect ) > timeout and state=1 \
  and closepolicy=1");

  std::cout << "SQL COMMAND:" << sqlCommand << std::endl;

  while (true) {
    try {
      DatabaseResult *result = mdatabaseVishnu->getResult(sqlCommand.c_str());

      if (result->getNbTuples() != 0) {
         for (size_t i = 0; i < result->getNbTuples(); ++i) {
          tmp.clear();
          tmp = result->get(i);

          ii=tmp.begin();
          SessionServer sessionServer (*ii);
          CommandServer commandServer (sessionServer);

          if (!commandServer.isRunning()) {
            sqlClosure.append("UPDATE vsession SET state=0 WHERE sessionkey='");
            sqlClosure.append(sessionServer.getData().getSessionKey());
            sqlClosure.append("';");

            sqlClosure.append("UPDATE vsession SET closure=CURRENT_TIMESTAMP WHERE sessionkey='");
            sqlClosure.append(sessionServer.getData().getSessionKey());
            sqlClosure.append("';");
          }
          std::cout << std::endl;
        }
        mdatabaseVishnu->process(sqlClosure.c_str());
        sqlClosure = "";
      }
      else {
        std::cout << "No sessions to close" << std::endl ;
      }
      sleep(minterval);

    } catch (VishnuException& e) {
      string errorInfo =  e.buildExceptionString();
      std::cout << "Exception: " << errorInfo <<std::endl;
    }
  }
  return 0;
}
