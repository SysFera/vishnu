/**
* \file MonitorTMS.cpp
* \brief This file presents the implementation of the TMS Monitor
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001
*/

#include <boost/scoped_ptr.hpp>
#include "ServerTMS.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "utilVishnu.hpp"
#include "MonitorTMS.hpp"

/**
* \brief Constructor, raises an exception on error
* \fn ServerTMS(std::string cfg)
* \param interval The interval to check the database
*/
MonitorTMS::MonitorTMS(int interval) {
minterval = interval;
mdatabaseVishnu = NULL;
}

/**
* \brief Destructor
* \fn ~MonitorTMS()
*/

MonitorTMS::~MonitorTMS() {
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
}


/**
* \brief To initialize the TMS monitor with individual parameters instead of configuration file
* \fn int init(int vishnuId, int dbType, std::string dbHost, std::string dbUsername, std::string dbPassword)
* \param vishnuId The password of the root user vishnu_user for the connection with the database
* \param dbConfig The configuration of the database
* \return raises an execption
*/
void
MonitorTMS::init(int vishnuId, DbConfiguration dbConfig, const std::string& machineId, const BatchType& batchType) {

  DbFactory factory;

  mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
  mmachineId = machineId;
  mbatchType = batchType;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(vishnuId));

  try {
    /*connection to the database*/
    mdatabaseVishnu->connect();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      throw SystemException(ERRCODE_DBERR, "The vishnuid is unrecognized");
    }
  } catch (VishnuException& e) {
    exit(0);
  }

}

/**
* \brief To launch the TMS Monitor
* \fn int run()
* \return raises an exception
*/
int
MonitorTMS::run() {

  std::vector<std::string>::iterator iter;
  std::vector<std::string> tmp;
  std::string batchJobId;
  std::string jobId;
  int state;
  std::string sqlUpdatedRequest;
  std::string sqlRequest = "SELECT jobId, batchJobId from job, vsession where vsession.numsessionid=job.vsession_numsessionid "
                           " and state >=0 and status < 4";

  try {

    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlRequest.c_str()));
    if (result->getNbTuples() != 0) {
      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        tmp.clear();
        tmp = result->get(i);

        iter = tmp.begin();
        jobId = *iter;
        iter++;
        batchJobId = *iter;
        std::cout << "MonitorTMS::run(): jobId = " << jobId  << ", batchJobId=" << batchJobId << std::endl;
        BatchFactory factory;
        boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(mbatchType));
        state = batchServer->getJobState(batchJobId);
        std::cout << "state = " << state << std::endl;
        sqlUpdatedRequest = "UPDATE job SET status="+vishnu::convertToString(state)+" where jobId='"+jobId+"'";
        std::cout << "MonitorTMS::run(): " << sqlUpdatedRequest << std::endl;
       
        mdatabaseVishnu->process(sqlUpdatedRequest.c_str()); 
        
      }
    }
    sleep(minterval);

  } catch (VishnuException& e) {
    string errorInfo =  e.buildExceptionString();
    std::cout << "errorInfo = " << errorInfo << std::endl;
  }
  return 0;
}
