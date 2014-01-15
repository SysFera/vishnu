/**
 * \file list_jobs.cpp
 * This file defines the VISHNU list jobs command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "GenericCli.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"
#include "constants.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param setJobIdFct: Function to set the id of the job
 * \param setNbCpuFct : Function to set the number of cpu allocated
 * \param fromDate : The start time of submission for the job
 * \param toDate : The end of submission date for the job
 * \param setOwnerFct : Function to set the owner of the job
 * \param status : Function to set the status of the job
 * \param setPriorityFct : Function to set the priority of the job
 * \param setQueueFct : Function to set the queue where the job where submitted
 * \param setMutipleStatusesFct : lists the jobs with the specified status (combination of multiple status)
 * \param setWorkIdFct: Function to set the job work id
 * \param configFile: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeListJobOp(string pgName,
              boost::function1<void, string>& setJobIdFct,
              boost::function1<void, int>& setNbCpuFct,
              string& fromDate,
              string& toDate,
              boost::function1<void, string>& setOwnerFct,
              string& status,
              boost::function1<void, TMS_Data::JobPriority>& setPriorityFct,
              boost::function1<void, string>& setQueueFct,
              boost::function1<void, string>& setMutipleStatusesFct,
              boost::function1<void, long long>& setWorkIdFct,
              string& configFile) {
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  // All cli obligatory parameters
  opt->add("jobId,i",
           "The id of the job",
           CONFIG,
           setJobIdFct);
  opt->add("nbCpu,P",
           "The number of cpu needed by the job",
           CONFIG,
           setNbCpuFct);
  opt->add("fromSubmitDate,d",
           "The start time for the job submission"
           "by providing the start date\n"
           "(in the format ""YYYY-MM-DD H:mm:ss "")",
           CONFIG,
           fromDate);
  opt->add("toSubmitDate,D",
           "The end time for the job submission"
           "by providing the end date \n"
           "(in the format ""YYYY-MM-DD H:mm:ss "")"
           "By default, the end date is the current day",
           CONFIG,
           toDate);
  opt->add("owner,u",
           "The owner of the job",
           CONFIG,
           setOwnerFct);
  opt->add("status,s",
           "The status of the job. The different values of job status are:\n"
           " 1 or S: for SUBMITTED job\n"
           " 2 or Q: for QUEUED job\n"
           " 3 or W: for WAITING job\n"
           " 4 or R: for RUNNING job\n"
           " 5 or T: for TERMINATED job\n"
           " 6 or C: for CANCELED job\n"
           " 7 or D: for DOWNLOADED job",
           CONFIG,
           status);
  opt->add("priority,p",
           "The priority of the job. The priority argument must be an integer between 1 and 5 inclusive",
           CONFIG,
           setPriorityFct);
  opt->add("queue,q",
           "List the submitted jobs in this queue",
           CONFIG,
           setQueueFct);
  opt->add("multipleStatus,S",
           "List the jobs with the specified status (combination of multiple status)",
           CONFIG,
           setMutipleStatusesFct);
  opt->add("workId,w",
           "Allows to gather information about jobs related to a given Work.",
           CONFIG,
           setWorkIdFct);

  return opt;
}


int
main (int argc, char* argv[]) {
  /******* Parsed value containers ****************/
  string configFile;
  string sessionKey;
  string machineId;
  string stateStr = "";
  int state;
  std::string fromDate;
  std::string toDate;

  /********** EMF data ************/
  TMS_Data::ListJobsOptions jobOp;

  /******** Callback functions ******************/
  boost::function1<void,string> setJobIdFct(boost::bind(&TMS_Data::ListJobsOptions::setJobId,boost::ref(jobOp),_1));
  boost::function1<void,int> setNbCpuFct(boost::bind(&TMS_Data::ListJobsOptions::setNbCpu,boost::ref(jobOp),_1));
  boost::function1<void,string> setOwnerFct(boost::bind(&TMS_Data::ListJobsOptions::setOwner,boost::ref(jobOp),_1));
  boost::function1<void,TMS_Data::JobPriority> setPriorityFct(boost::bind(&TMS_Data::ListJobsOptions::setPriority,boost::ref(jobOp),_1));
  boost::function1<void,string> setQueueFct(boost::bind(&TMS_Data::ListJobsOptions::setQueue,boost::ref(jobOp),_1));
  boost::function1<void,string> setMultipleStatusesFct(boost::bind(&TMS_Data::ListJobsOptions::setMultipleStatus,boost::ref(jobOp),_1));
  boost::function1<void,long long> setWorkIdFct(boost::bind(&TMS_Data::ListJobsOptions::setWorkId,boost::ref(jobOp),_1));

  /*********** Out parameters *********************/
  TMS_Data::ListJobs job;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeListJobOp(argv[0],
      setJobIdFct,
      setNbCpuFct,
      fromDate,
      toDate,
      setOwnerFct,
      stateStr,
      setPriorityFct,
      setQueueFct,
      setMultipleStatusesFct,
      setWorkIdFct,
                                               configFile);

  opt->add("isBatchJob,b",
           "allows to select all jobs submitted  through the underlying"
           " batch scheduler (jobs submitted through vishnu and out of vishnu)",
           CONFIG);

  opt->add("isListAll,l",
           "allows to list all information",
           CONFIG);

  opt->add("machineId,m",
           "represents the id of the machine",
           HIDDEN,
           machineId,1);
  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  // Process command
  try {
    if (stateStr.size()!=0) {
      size_t pos = stateStr.find_first_not_of("0123456789");
      if (pos!=std::string::npos) {
        if (stateStr.size() == 1) {
          switch(stateStr[0]) {
          case 'S' :
            state = vishnu::STATE_SUBMITTED;
            break;
          case 'Q' :
            state = vishnu::STATE_QUEUED;
            break;
          case 'W' :
            state = vishnu::STATE_WAITING;
            break;
          case 'R' :
            state = vishnu::STATE_RUNNING;
            break;
          case 'T' :
            state = vishnu::STATE_COMPLETED;
            break;
          case 'C' :
            state = vishnu::STATE_CANCELLED;
            break;
          case 'D' :
            state = vishnu::STATE_DOWNLOADED;
            break;
          case 'F' :
            state = vishnu::STATE_FAILED;
            break;
          default:
            state = vishnu::STATE_UNDEFINED;
            break;
          }
        }
        if (stateStr.size() > 1 || state < 0) {
          std::cerr <<  (boost::format("Unknown job state: %1%")%stateStr).str();
          return CLI_ERROR_INVALID_PARAMETER;
        }
      } else {
        state = convertToInt(stateStr);
      }
      jobOp.setStatus(state);
    }
    //convert the date in long format
    if (opt->count("fromSubmitDate")) {
      jobOp.setFromSubmitDate(convertLocaltimeINUTCtime(string_to_time_t(fromDate)));
    }

    if (opt->count("toSubmitDate")) {
      jobOp.setToSubmitDate(convertLocaltimeINUTCtime(string_to_time_t(toDate)));
    }

    if (opt->count("isBatchJob")) {
      jobOp.setBatchJob(true);
    }

    if (opt->count("isListAll")) {
      jobOp.setListAll(true);
    }

    if (vishnuInitialize(const_cast<char*>(configFile.c_str()), argc, argv)) {
      errorUsage(argv[0],communicationErrorMsg,EXECERROR);
      return  CLI_ERROR_COMMUNICATION ;
    }

    // get the sessionKey
    sessionKey = getLastSessionKey(getppid());

    // VISHNU call : submit
    if (!sessionKey.empty()) {
      listJobs(sessionKey, machineId, job, jobOp);
    }
    if (jobOp.getOwner().size() == 0 && jobOp.getJobId().size() == 0  && jobOp.getNbCpu() <= 0
        && jobOp.getFromSubmitDate() <= 0 && jobOp.getToSubmitDate() <= 0 && !jobOp.isListAll()) {
      std::cout << job << std::endl;
    } else {
      displayListJobs(job);
    }
  } catch(VishnuException& e) {// catch all Vishnu runtime error
    std::string  msg = e.getMsg();
    if (e.getMsgComp().compare("Bus initialization failure")==0){
      msg += " ["+e.getMsgComp()+", machineId may be invalid]";
    } else {
      msg += " ["+e.getMsgComp()+"]";
    }

    errorUsage(argv[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)) {
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e) {// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }
  return 0;
}
