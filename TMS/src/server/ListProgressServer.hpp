/**
 * \file ListProgressServer.hpp
 * \brief This file contains the VISHNU ListProgressServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#ifndef _LIST_PROGRESS_SERVER_
#define _LIST_PROGRESS_SERVER_

#include <string>
#include <vector>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/local_time/posix_time_zone.hpp>
#include <boost/date_time/date_facet.hpp>
#include <sstream>

#include "QueryServer.hpp"
#include "TMS_Data.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "constants.hpp"
#include "utilServer.hpp"

/**
 * \class ListProgressServer
 * \brief ListProgressServer class implementation
 */
class ListProgressServer: public QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression> {

public:

  /**
   * \brief Constructor, raises an exception on error
   */
  ListProgressServer(const std::string& authkey)
    : QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(),
      mcommandName("vishnu_get_job_progress")
  {
    vishnu::validateAuthKey(authkey, mdatabase, muserSessionInfo);
  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListProgression*
  list(TMS_Data::ProgressOptions_ptr options) {

    std::vector<std::string> results;
    std::vector<std::string>::iterator iter;
    std::string batchJobId;
    int status;
    long startTime;
    long walltime;

    TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListProgression();

    std::string query = "SELECT jobId, jobName, wallClockLimit, endDate, status, batchJobId"
                        " FROM vsession, job"
                        " WHERE vsession.numsessionid=job.vsession_numsessionid";

    if (! options->getMachineId().empty()){
      query += "  AND job.machine_nummachineid='"+ getNumMachine(options->getMachineId()) +"'";
    }

    if (! options->getJobId().empty()) {
      std::string jobId = options->getJobId();
      query.append(" AND jobId='"+mdatabase->escapeData(jobId)+"'");
      boost::scoped_ptr<DatabaseResult> sqlResult(ServerXMS::getInstance()->getDatabaseVishnu()->getResult(query.c_str()));
      if (sqlResult->getNbTuples() == 0) {
        throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
      }
    } else {
      query.append(" AND owner='"+mdatabase->escapeData(options->getUser())+"'");
    }

    query.append("  AND status < 5 ORDER BY jobId");

    boost::scoped_ptr<DatabaseResult> sqlResult(ServerXMS::getInstance()->getDatabaseVishnu()->getResult(query.c_str()));

    if (sqlResult->getNbTuples() != 0){
      for (size_t entryIndex = 0; entryIndex < sqlResult->getNbTuples(); ++entryIndex) {
        results.clear();
        results = sqlResult->get(entryIndex);
        iter = results.begin();

        TMS_Data::Progression_ptr job = ecoreFactory->createProgression();

        job->setJobId(*iter);
        job->setJobName(*(++iter));
        walltime = vishnu::convertToInt(*(++iter));
        job->setWallTime(walltime);
        job->setEndTime(vishnu::convertToLong(*(++iter)));
        status = vishnu::convertToInt(*(++iter));
        job->setStatus(status);
        batchJobId = *(++iter);

        BatchFactory factory;
        BatchType batchType  = ServerXMS::getInstance()->getBatchType();
        std::string batchVersion  = ServerXMS::getInstance()->getBatchVersion();
        boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType,
                                                                                  batchVersion));

        startTime = batchServer->getJobStartTime(batchJobId);
        if(startTime!=0) {
          job->setStartTime(startTime);

          if (status == vishnu::STATE_COMPLETED) {
            job->setPercent(100);
          } else if(status == vishnu::STATE_RUNNING) {
            time_t currentTime = vishnu::getCurrentTimeInUTC();
            int percent = 0;
            time_t gap = currentTime-startTime;
            if (walltime == 0) {
              walltime = 60;
            }

            if (gap < walltime) {
              double ratio =  100*(double(gap)/walltime);
              if(ratio > 0.0 && ratio <= 1.0) {
                percent = 1;
              } else {
                percent = static_cast<int>(ratio);
              }
            } else {
              percent = 99;
            }
            job->setPercent(percent);
          } else {
            job->setPercent(0);
          }
        } else {
          job->setPercent(0);
        }
        mlistObject->getProgress().push_back(job);
      }
    }

    mlistObject->setNbJobs(mlistObject->getProgress().size());

    return mlistObject;

  }


  /**
   * \brief Function to get the name of the ListProgressServer command line
   * \return The the name of the ListProgressServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListProgressServer()
  {
  }

private:

  /**
  * \brief The name of the ListProgressServer command line
  */
  std::string mcommandName;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;
};

#endif
