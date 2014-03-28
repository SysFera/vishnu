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

  BatchFactory factory;
  boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(mbatchType, mbatchVersion));
  while (kill(getppid(), 0) == 0) {
    try {

      std::string sqlQuery = boost::str(
                               boost::format(
                                 "SELECT jobId, batchJobId, vmIp, vmId, owner "
                                 " FROM job, vsession "
                                 " WHERE vsession.numsessionid=job.vsession_numsessionid "
                                 " AND submitMachineId='%1%'"
                                 " AND batchType=%2%"
                                 " AND status >= %3% "
                                 " AND status < %4%;")
                               % mmachineId
                               % vishnu::convertToString(mbatchType)
                               % vishnu::STATE_UNDEFINED
                               % vishnu::STATE_COMPLETED);

      boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlQuery.c_str()));
      if (result->getNbTuples() == 0) {
        sleep(minterval);
        continue;
      }

      std::vector<std::string> buffer;
      std::vector<std::string>::iterator item;

      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        buffer.clear();
        buffer = result->get(i);
        item = buffer.begin();

        TMS_Data::Job job;
        job.setJobId( *item++ );
        job.setBatchJobId( *item++ );
        job.setVmIp( *item++ );
        job.setVmId( *item++ );
        job.setOwner( *item );

        try {
          int state;
          switch (mbatchType) {
          case DELTACLOUD:
          case OPENNEBULA:
            state = batchServer->getJobState( JsonObject::serialize(job) );
            break;
          default:
            state = batchServer->getJobState(job.getBatchJobId());
            break;
          }
          std::string query = boost::str(boost::format("UPDATE job SET status=%1%"
                                                       " WHERE jobId='%2%';")
                                         % vishnu::convertToString(state) % job.getJobId());
          if (state == vishnu::STATE_COMPLETED) {
            query.append(boost::str(boost::format("UPDATE job SET endDate=CURRENT_TIMESTAMP"
                                                  " WHERE jobId='%1%';") % job.getJobId()));
          }
          mdatabaseVishnu->process(query.c_str());
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
