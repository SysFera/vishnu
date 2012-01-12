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
#include "boost/date_time/posix_time/posix_time.hpp"


#include "QueryServer.hpp"
#include "SessionServer.hpp"
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
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param machineId The identifier of the machine in which the jobs will be listed
   * \brief Constructor, raises an exception on error
   */
  ListJobServer(const SessionServer session, std::string machineId):
    QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>(session) {
   mcommandName = "vishnu_list_jobs";
   mmachineId = machineId;
  }
  /**
   * \param params The object which encapsulates the information of ListJobServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param machineId The identifier of the machine in which the jobs will be listed
   * \brief Constructor, raises an exception on error
   */
  ListJobServer(TMS_Data::ListJobsOptions_ptr params, const SessionServer& session, std::string machineId):
    QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>(params, session) {
    mcommandName = "vishnu_list_jobs";
    mmachineId = machineId;
  }

  /**
   * \brief Function to treat the listJobServer options
   * \param options the object which contains the ListJobServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(const TMS_Data::ListJobsOptions_ptr& options, std::string& sqlRequest) {

    //To check if the jobId is defined
    if (options->getJobId().size() != 0) {
      //To check if the jobIf exists
      checkJobId(options->getJobId());
      //To add the jobId on the request
      addOptionRequest("jobId", options->getJobId(), sqlRequest);
    }

    //To check if the number of cpu is defined and positive
    if (options->getNbCpu() > 0) {
      //To add the number of the cpu to the request
      addOptionRequest("nbCpus", convertToString(options->getNbCpu()), sqlRequest);
    } else {
      //If the options is not the default value -1
      if (options->getNbCpu() != -1) {
        throw UserException(ERRCODE_INVALID_PARAM, "The number of cpu is incorrect");
      }
    }

    time_t fromSubmitDate = static_cast<time_t>(options->getFromSubmitDate());
    if(fromSubmitDate != -1) {
      fromSubmitDate = convertUTCtimeINLocaltime(fromSubmitDate);
      std::string submitDateStr =  boost::posix_time::to_simple_string(boost::posix_time::from_time_t(fromSubmitDate));
      addTimeRequest("submitDate", submitDateStr, sqlRequest, ">=");
    }

    time_t toSubmitDate = static_cast<time_t>(options->getToSubmitDate());
    if(toSubmitDate != -1) {
      toSubmitDate = convertUTCtimeINLocaltime(toSubmitDate); 
      std::string submitDateStr =  boost::posix_time::to_simple_string(boost::posix_time::from_time_t(toSubmitDate));
      addTimeRequest("submitDate", submitDateStr, sqlRequest, "<=");
    }

    //To check the job status
    if (options->getStatus() != -1) {
      //To check the job status options
      checkJobStatus(options->getStatus());
      //To add the number of the cpu to the request
      addOptionRequest("status", convertToString(options->getStatus()), sqlRequest);
    } else {
       if (options->getJobId().empty() && options->getMultipleStatus().empty()) { 
         sqlRequest.append(" and status < 5 "); 
       } 
    }

    if(!options->getMultipleStatus().empty()) {
      bool error = false;
      string invalidState;
      std::string multStat = options->getMultipleStatus();
      std::string::iterator iter;
      std::string::iterator beg = multStat.begin();
      std::string::iterator end = multStat.end();
      std::vector<std::string> vec;
      for(iter=beg; iter!=end; ++iter) {
        switch(*iter) {
          case 'S':case '1':
            vec.push_back("status=1");
            break;
          case 'Q':case '2':
            vec.push_back("status=2");
            break;
          case 'W':case '3':
            vec.push_back("status=3");
            break;
          case 'R':case '4':
            vec.push_back("status=4");
            break;
          case 'T':case '5':
            vec.push_back("status=5");
            break;
          case 'C':case '6':
            vec.push_back("status=6");
            break;
          case 'D':case '7':
            vec.push_back("status=7");
            break;
          default:
            error = true;
            invalidState = *iter;
            break;
        }

        if(error) {
          throw UserException(ERRCODE_INVALID_PARAM, invalidState+" is an invalid state.");
        }
      }
      std::vector<std::string>::iterator vec_iter;
      if(!vec.empty()){
        sqlRequest.append(" and ("+*vec.begin());
        for(vec_iter=vec.begin()+1; vec_iter!=vec.end(); ++vec_iter) {

          sqlRequest.append(" or "+*vec_iter);
        }
        sqlRequest.append(")");
      }
    }

    //To check the job priority
    if (options->getPriority() != -1) {
      //To check the job priority options
      checkJobPriority(options->getPriority());
      //To add the number of the cpu to the request
      addOptionRequest("jobPrio", convertToString(options->getPriority()), sqlRequest);
    }

    if (options->getOwner().size() != 0) {
      addOptionRequest("owner", options->getOwner(), sqlRequest);
    }

    //To check if the queue is defined
    if (options->getQueue().size() != 0) {
      BatchFactory factory;
      BatchType batchType  = ServerTMS::getInstance()->getBatchType();
      boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));
      batchServer->listQueues(options->getQueue()); //raise an exception if options->getQueue does not exist
 
      addOptionRequest("jobQueue", options->getQueue(), sqlRequest);
    }
  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListJobs*
  list() {
    std::string sqlListOfJobs = "SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, jobPath,"
                                "outputPath, errorPath, jobPrio, nbCpus, jobWorkingDir, status, submitDate, endDate, owner,"
                                "jobQueue,wallClockLimit, groupName, jobDescription, memLimit, nbNodes, "
                                "nbNodesAndCpuPerNode, batchJobId from job, vsession "
                                "where vsession.numsessionid=job.vsession_numsessionid"
                                " and status > 0 ";

    if(mmachineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)!=0) {
      sqlListOfJobs.append(" and job.submitMachineId='"+mmachineId+"'");
    }

    bool allOptionsAreNotSet = (mparameters->getNbCpu()<=0);
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getFromSubmitDate()==-1);
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getToSubmitDate()==-1);
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getStatus()==-1);
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getMultipleStatus().empty());
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getPriority()==-1);
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getOwner().empty());
    allOptionsAreNotSet = allOptionsAreNotSet && (mparameters->getQueue().empty());

    if(mparameters->isBatchJob() && !allOptionsAreNotSet) {
     throw UserException(ERRCODE_INVALID_PARAM, "Conflict: the batchJob option is incompatible with other options excepted jobId option.");
    }

    if(mparameters->isBatchJob() && mmachineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)==0) {
     throw UserException(ERRCODE_INVALID_PARAM, "Conflict: the batchJob option is incompatible with machine id equal to all.");
    }

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;

    TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListJobs();

    msessionServer.check();

    processOptions(mparameters, sqlListOfJobs);
    sqlListOfJobs.append(" order by submitDate");

    boost::scoped_ptr<DatabaseResult> ListOfJobs (mdatabaseVishnu->getResult(sqlListOfJobs.c_str()));
    long nbRunningJobs = 0;
    long nbWaitingJobs = 0;
    int jobStatus ;
    time_t submitDate;
    time_t endDate;
    std::vector<string> ignoredIds;

    if (ListOfJobs->getNbTuples() != 0){
      for (size_t i = 0; i < ListOfJobs->getNbTuples(); ++i) {
        results.clear();
        results = ListOfJobs->get(i);
        ii = results.begin();

        TMS_Data::Job_ptr job = ecoreFactory->createJob();

        job->setSessionId(*ii);
        job->setSubmitMachineId(*(++ii));
        job->setSubmitMachineName(*(++ii));
        job->setJobId(*(++ii));
        job->setJobName(*(++ii));
        job->setJobPath(*(++ii));
        job->setOutputPath(*(++ii));
        job->setErrorPath(*(++ii));
        job->setJobPrio(convertToInt(*(++ii)));
        job->setNbCpus(convertToInt(*(++ii)));
        job->setJobWorkingDir(*(++ii));
        jobStatus = convertToInt(*(++ii));
        if(jobStatus==4) {
          nbRunningJobs++;
        } else if(jobStatus >= 1 && jobStatus <= 3) {
          nbWaitingJobs++;
        }
        job->setStatus(jobStatus);
        //convert the endDate into UTC date
        submitDate = convertLocaltimeINUTCtime(convertToTimeType(*(++ii)));
        job->setSubmitDate(submitDate);
        //convert the endDate into UTC date
        endDate = convertLocaltimeINUTCtime(convertToTimeType(*(++ii)));
        job->setEndDate(endDate);
        job->setOwner(*(++ii));
        job->setJobQueue(*(++ii));
        job->setWallClockLimit(convertToInt(*(++ii)));
        job->setGroupName(*(++ii));
        job->setJobDescription(*(++ii));
        job->setMemLimit(convertToInt(*(++ii)));
        job->setNbNodes(convertToInt(*(++ii)));
        job->setNbNodesAndCpuPerNode(*(++ii)); 

        ignoredIds.push_back(*(++ii));

        mlistObject->getJobs().push_back(job);
      }
      mlistObject->setNbJobs(mlistObject->getJobs().size());
      mlistObject->setNbRunningJobs(nbRunningJobs);
      mlistObject->setNbWaitingJobs(nbWaitingJobs);
    } 
 
    if(mparameters->isBatchJob() && !mparameters->getJobId().empty() && mmachineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)!=0){
      BatchFactory factory;
      BatchType batchType  = ServerTMS::getInstance()->getBatchType();
      boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));

      if(mlistObject != NULL) {
        std::vector<std::string>::const_iterator iter = ignoredIds.begin();
        for(unsigned int i = 0; i < mlistObject->getJobs().size(); i++) {
          if(iter!=ignoredIds.end()){
            (mlistObject->getJobs().get(i))->setJobId(*iter);
            int state = batchServer->getJobState(*iter);
            ++iter;
            if(state!=-1 && (mlistObject->getJobs().get(i))->getStatus()!=5 &&
                (mlistObject->getJobs().get(i))->getStatus()!=6 &&
                (mlistObject->getJobs().get(i))->getStatus()!=7) {
              (mlistObject->getJobs().get(i))->setStatus(state);
            }
          }
        }
      }
      return mlistObject;
    }

    if(mparameters->isBatchJob() && mmachineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)!=0 && allOptionsAreNotSet){
      BatchFactory factory;
      BatchType batchType  = ServerTMS::getInstance()->getBatchType();
      boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));

      batchServer->fillListOfJobs(mlistObject, ignoredIds);
    }

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

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListJobServer command line
  */
  std::string mcommandName;
  /**
  * \brief The name of the machine in which the jobs whill be listed
  */
  std::string mmachineId;
};

#endif
