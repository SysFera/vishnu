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

#include <csignal>

#include <boost/format.hpp>

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
   * \param batchVersion The batch scheduler version
 * \return raises an execption
 */
void
MonitorTMS::init(int vishnuId, DbConfiguration dbConfig, const std::string& machineId,
                 const BatchType& batchType, const std::string& batchVersion) {

  DbFactory factory;

  mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
  mmachineId = machineId;
  mbatchType = batchType;
  mbatchVersion = batchVersion;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+vishnu::convertToString(vishnuId));

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
 * \return raises exception on erro
 */
void
MonitorTMS::run() {
  int state;
  std::string query;
  std::string sqlRequest = "SELECT jobId, batchJobId, vmIp, vmId "
                           " FROM job, vsession "
                           " WHERE vsession.numsessionid=job.vsession_numsessionid "
                           " AND status > 0 "
                           " AND status < 5 "
                           " AND submitMachineId='"+mmachineId+"'"
                           " AND batchType="+vishnu::convertToString(mbatchType);

  std::string vmUser = "root";
  if (mbatchType == DELTACLOUD) {
    vmUser = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], true, "root");
  }

  BatchFactory factory;
  boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(mbatchType, mbatchVersion));
  while (kill(getppid(), 0) == 0) {
    try {
      boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlRequest.c_str()));
      if (result->getNbTuples() == 0) {
        sleep(minterval);
        continue;
      }

      std::string jobId;
      std::string batchJobId;
      std::string vmIp;
      std::string vmId;
      std::vector<std::string> buffer;
      std::vector<std::string>::iterator item;

      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        buffer.clear();
        buffer = result->get(i);
        item = buffer.begin();
        jobId = *item;
        ++item; batchJobId = *item;
        ++item; vmIp = *item;
        ++item; vmId = *item;
        std::string jobDescr = (mbatchType == DELTACLOUD)? batchJobId+"@"+vmUser+"@"+vmIp+"@"+vmId : batchJobId;
        try {
          state = batchServer->getJobState(jobDescr);
          if (state != vishnu::STATE_UNDEFINED) {
            query = (boost::format("UPDATE job SET status=%1%"
                                   " WHERE jobId='%2%';")
                     %vishnu::convertToString(state)
                     %jobId).str();
            if (state == vishnu::STATE_COMPLETED) {
              query.append((boost::format("UPDATE job SET endDate=CURRENT_TIMESTAMP"
                                          " WHERE jobId='%1%';")%jobId).str());
            }
            mdatabaseVishnu->process(query.c_str());
          }
        } catch (VishnuException& ex) {
          std::clog << boost::format("[TMSMONITOR][ERROR] %1%\n")%ex.what();
        }
      }
    } catch (VishnuException& ex) {
      std::clog << boost::format("[TMSMONITOR][ERROR] %1%\n")%ex.what();
    }
    sleep(minterval);
  }
}
