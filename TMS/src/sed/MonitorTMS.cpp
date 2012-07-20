/**
* \file MonitorTMS.cpp
* \brief This file presents the implementation of the TMS Monitor
* \authors Daouda Traore (daouda.traore@sysfera.com) and
* Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date April
*/

#include <boost/scoped_ptr.hpp>
#include "ServerTMS.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "MonitorTMS.hpp"

/**
* \brief Constructor, raises an exception on error
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
 * \param vishnuId The password of the root user vishnu_user for the connection with the database
 * \param dbConfig The configuration of the database
 * \param machineId The machine identifier
 * \param batchType The batch scheduler type
 * \return raises an execption
*/
void
MonitorTMS::init(int vishnuId, DbConfiguration dbConfig, const std::string& machineId, const BatchType& batchType) {

  DbFactory factory;

  mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
  mmachineId = machineId;
  mbatchType = batchType;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+vishnu::convertToString(vishnuId));

  try {
    /*connection to the database*/
    mdatabaseVishnu->connect();

    /* Checking of vishnuid on the database */
    SOCISession session = mdatabaseVishnu->getSingleSession();
    session.execute(sqlCommand);
    bool got_data = session.got_data();
    mdatabaseVishnu->releaseSingleSession(session);
    if( ! got_data ) {
        throw SystemException(ERRCODE_DBERR, "The vishnuid is unrecognized");
    }
  } catch (VishnuException& e) {
    exit(0);
  }

}

/**
* \brief To launch the TMS Monitor
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
                           " and status > 0 and status < 5 and submitMachineId='"+mmachineId+"'";

  try {

    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlRequest.c_str()));
    if (result->getNbTuples() != 0) {
      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        tmp.clear();
        tmp = result->get(i);

        iter = tmp.begin();
        jobId = *iter;
        ++iter;
        batchJobId = *iter;
        BatchFactory factory;
        boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(mbatchType));
        
        state = batchServer->getJobState(batchJobId);
        if(state!=-1) {
          sqlUpdatedRequest = "UPDATE job SET status="+vishnu::convertToString(state)+" where jobId='"+jobId+"'";
         
          mdatabaseVishnu->process(sqlUpdatedRequest.c_str()); 

          if(state==5) {
            sqlUpdatedRequest = "UPDATE job SET endDate=CURRENT_TIMESTAMP where jobId='"+jobId+"'";
            mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
          }
        }
      }
    }
    sleep(minterval);

  } catch (VishnuException& e) {
    string errorInfo =  e.buildExceptionString();
    if (e.getMsgI() == ERRCODE_DBERR) {
      cerr << errorInfo << endl;
      exit(1);
    }
    sleep(minterval);
  }
  return 0;
}
