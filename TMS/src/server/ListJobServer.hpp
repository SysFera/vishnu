/**
 * \file ListJobServer.hpp
 * \brief This file contains the VISHNU ListJobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_JOB_SERVER_
#define _LIST_JOB_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <boost/foreach.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "ServerXMS.hpp"
#include "QueryServer.hpp"
#include "TMS_Data.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"

/**
 * \class ListJobServer
 * \brief ListJobServer class implementation
 */
class ListJobServer: public QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs> {

public:

  /**
   * \param machineId The identifier of the machine in which the jobs will be listed
   * \brief Constructor, raises an exception on error
   */
  ListJobServer(std::string authkey)
    : QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>() {
    mcommandName = "vishnu_list_jobs";
    vishnu::validateAuthKey(authkey, mdatabase, muserSessionInfo);
  }

  /**
   * \brief Function to treat the listJobServer options
   * \param options the object which contains the ListJobServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(const TMS_Data::ListJobsOptions_ptr& options, std::string& query) {

    // check machine option
    if (! options->getMachineId().empty()) {
      std::string cond = boost::str(
                           boost::format(" AND job.machine_nummachineid='%1%'")
                           % getNumMachine(options->getMachineId()));
      query.append(cond);
    }

    if (! options->getOwner().empty()) {
      std::string cond = boost::str(
                           boost::format(" AND users.userid = '%1%'")
                           % mdatabase->escapeData(options->getOwner()));
      query.append(cond);
    }

    //To check if the jobId is defined
    if (! options->getJobId().empty()) {
      std::string cond = boost::str(
                           boost::format(" AND job.id = '%1%'")
                           % vishnu::convertToLong(options->getJobId())) ;
      query.append(cond);
    }

    //To check if the number of cpu is defined and positive
    if (options->getNbCpu() > 0) {
      std::string cond = boost::str(
                           boost::format(" AND job.nbCpus = %1%")
                           % options->getNbCpu()) ;
      query.append(cond);
    }

    if (options->getFromSubmitDate() > 0) {
      std::string cond = boost::str(
                           boost::format(" AND job.submitDate >= '%1%'")
                           % vishnu::timeToTimestamp(options->getFromSubmitDate() ));
      query.append(cond);
    }

    if (options->getToSubmitDate() > 0) {
      std::string cond = boost::str(
                           boost::format(" AND job.submitDate <= '%1%'")
                           % vishnu::timeToTimestamp(options->getToSubmitDate()) );
      query.append(cond);
    }

    //Check the job status
    if (options->getStatus() > 0) {
      std::string cond = boost::str(
                           boost::format(" AND job.status = %1%")
                           % options->getStatus());
      query.append(cond);
    } else {
      if (options->getJobId().empty() && options->getMultipleStatus().empty()) {
        std::string cond = boost::str(
                             boost::format(" AND job.status < %1%")
                             % vishnu::STATE_COMPLETED);
        query.append(cond);
      }
    }

    // append multiple status conditions
    query.append( buildMultipleStatusConditions(options->getMultipleStatus()) );

    //To check if the queue is defined
    if (! options->getQueue().empty()) {
      std::string cond = boost::str(
                           boost::format(" AND job.jobQueue = '%1%'")
                           % mdatabase->escapeData(options->getQueue()));
      query.append(cond);
    }

    //To check work id
    if (! options->getQueue().empty()) {
      std::string cond = boost::str(
                           boost::format(" AND job.work_id = %1%")
                           % options->getWorkId());
      query.append(cond);
    }
  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListJobs*
  list(TMS_Data::ListJobsOptions_ptr options)
  {
    std::string query ="SELECT DISTINCT vsession.vsessionid, job.id, batchJobId, job.name, users.userid, account.aclogin, "
                       "machine.machineid, scriptpath, outputPath, errorPath, nbCpus, workingdir, "
                       "job.status, submitDate, endDate, queue, wallClockLimit, priority, groupName, "
                       "job.description, memLimit, nbNodes, nbNodesAndCpuPerNode, work_id"
                       " FROM job, vsession, users, machine, account"
                       " WHERE job.vsession_numsessionid = vsession.numsessionid"
                       "  AND  job.users_numuserid       = users.numuserid"
                       "  AND  job.machine_nummachineid  = account.machine_nummachineid";

    if (! options->isListAll()) {
      processOptions(options, query);
    }

    query.append(" ORDER BY submitDate;");

    boost::scoped_ptr<DatabaseResult> ListOfJobs ( mdatabase->getResult(query) );

    long nbRunningJobs = 0;
    long nbWaitingJobs = 0;
    TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListJobs();

    for (size_t jobEntry = 0; jobEntry < ListOfJobs->getNbTuples(); ++jobEntry) {
      std::vector<std::string> results = ListOfJobs->get(jobEntry);
      std::vector<std::string>::iterator curResultEntry = results.begin();

      TMS_Data::Job_ptr job = ecoreFactory->createJob();

      job->setSessionId(*curResultEntry);
      job->setId(*(++curResultEntry));
      job->setBatchJobId(*(++curResultEntry));
      job->setName(*(++curResultEntry));
      job->setUserId(*(++curResultEntry));
      job->setLocalAccount(*(++curResultEntry));
      job->setMachine(*(++curResultEntry));
      job->setPath(*(++curResultEntry));
      job->setOutputPath(*(++curResultEntry));
      job->setErrorPath(*(++curResultEntry));
      job->setNbCpus(vishnu::convertToInt(*(++curResultEntry)));
      job->setWorkingDir(*(++curResultEntry));
      job->setStatus(vishnu::convertToInt(*(++curResultEntry)));
      job->setSubmitDate( vishnu::string_to_time_t(*(++curResultEntry)) );
      job->setEndDate( vishnu::string_to_time_t(*(++curResultEntry)) );
      job->setQueue(*(++curResultEntry));
      job->setWallClockLimit(vishnu::convertToInt(*(++curResultEntry)));
      job->setPriority(vishnu::convertToInt(*(++curResultEntry)));
      job->setGroupName(*(++curResultEntry));
      job->setDescription(*(++curResultEntry));
      job->setMemLimit(vishnu::convertToInt(*(++curResultEntry)));
      job->setNbNodes(vishnu::convertToInt(*(++curResultEntry)));
      job->setNbNodesAndCpuPerNode(*(++curResultEntry));
      job->setWorkId(vishnu::convertToLong(*(++curResultEntry)));

      mlistObject->getJobs().push_back(job);

      // update stats
      if (job->getStatus() == vishnu::STATE_RUNNING) {
        ++nbRunningJobs;
      } else if(job->getStatus() >= vishnu::STATE_SUBMITTED && job->getStatus() <= vishnu::STATE_WAITING) {
        ++nbWaitingJobs;
      }
    }
    mlistObject->setNbJobs(mlistObject->getJobs().size());
    mlistObject->setNbRunningJobs(nbRunningJobs);
    mlistObject->setNbWaitingJobs(nbWaitingJobs);
    return mlistObject;
  }


  /**
   * \brief Function to get the name of the ListJobServer command line
   * \return The the name of the ListJobServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListJobServer()
  {
  }

private:
  /**
   * @brief Build condition if multiple status required
   * @param optionValue The value of the option
   * @return
   */
  std::string
  buildMultipleStatusConditions(const std::string& optionValue)
  {
    std::string result = "";

    if(! optionValue.empty()) {
      std::vector<std::string> conditions;
      for (std::string::const_iterator iter=optionValue.begin(), end = optionValue.end(); iter!=end; ++iter) {
        switch(*iter) {
          case 'S':
          case 48+vishnu::STATE_SUBMITTED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_SUBMITTED));
            break;
          case 'Q':
          case 48+vishnu::STATE_QUEUED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_QUEUED));
            break;
          case 'W':
          case 48+vishnu::STATE_WAITING:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_WAITING));
            break;
          case 'R':
          case 48+vishnu::STATE_RUNNING:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_RUNNING));
            break;
          case 'T':
          case 48+vishnu::STATE_COMPLETED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_COMPLETED));
            break;
          case 'C':
          case 48+vishnu::STATE_CANCELLED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_CANCELLED));
            break;
          case 'D':
          case 48+vishnu::STATE_DOWNLOADED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_DOWNLOADED));
            break;
          case 'F':
          case 48+vishnu::STATE_FAILED:
            conditions.push_back("job.status="+vishnu::convertToString(vishnu::STATE_FAILED));
            break;
          default:
            throw UserException(ERRCODE_INVALID_PARAM,
                                boost::str(boost::format("Unknown job state: %1%")%*iter));
            break;
        }
      }

      if (! conditions.empty()) {
        result.append(" AND ("+*conditions.begin());
        for (std::vector<std::string>::iterator
             stIter = conditions.begin() + 1, statusVectorEnd = conditions.end();
             stIter != statusVectorEnd;
             ++stIter) {
          result.append(" OR "+*stIter);
        }
        result.append(")");
      }
    }

    return result;
  }


  /**
   * \brief The name of the ListJobServer command line
   */
  std::string mcommandName;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;

};

#endif
