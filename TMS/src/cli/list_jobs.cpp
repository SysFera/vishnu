/**
 * \file list_jobs.cpp
 * This file defines the VISHNU list jobs command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "displayer.hpp"
#include "constants.hpp"
#include <boost/format.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param fjid: The id of the job
 * \param fnbCpu : The number of cpu needed
 * \param fstart : The start time of submission for the job
 * \param fend : The end of submission date for the job
 * \param fowner : The owner of the job
 * \param status : The status of the job
 * \param fpriority : The priority of the job
 * \param foutput : The output path
 * \param ferr : The error path
 * \param fqueue : The queue to set the job
 * \param fmutlStatus : lists the jobs with the specified status (combination of multiple status)
 * \param configFile: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeListJobOp(string pgName,
              boost::function1<void, string>& fjid,
              boost::function1<void, int>& fnbCpu,
              string& fromDate,
              string& toDate,
              boost::function1<void, string>& fowner,
              string& status,
              boost::function1<void, JobPriority>& fpriority,
              boost::function1<void, string>& fqueue,
              boost::function1<void, string>& fmutlStatus,
              boost::function1<void, long long>& fworkId,
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
           fjid);
  opt->add("nbCpu,P",
           "The number of cpu needed by the job",
           CONFIG,
           fnbCpu);
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
           fowner);
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
           fpriority);
  opt->add("queue,q",
           "List the submitted jobs in this queue",
           CONFIG,
           fqueue);
  opt->add("multipleStatus,S",
           "List the jobs with the specified status (combination of multiple status)",
           CONFIG,
           fmutlStatus);
  opt->add("workId,w",
           "Allows to gather information about jobs related to a given Work.",
           CONFIG,
           fworkId);

  return opt;
}


int
main (int argc, char* argv[]) {
  int ret; // Return value

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
  boost::function1<void,string> fjid(boost::bind(&TMS_Data::ListJobsOptions::setJobId,boost::ref(jobOp),_1));
  boost::function1<void,int> fnbCpu(boost::bind(&TMS_Data::ListJobsOptions::setNbCpu,boost::ref(jobOp),_1));
  boost::function1<void,string> fown(boost::bind(&TMS_Data::ListJobsOptions::setOwner,boost::ref(jobOp),_1));
  boost::function1<void,JobPriority> fpriority(boost::bind(&TMS_Data::ListJobsOptions::setPriority,boost::ref(jobOp),_1));
  boost::function1<void,string> fqueue(boost::bind(&TMS_Data::ListJobsOptions::setQueue,boost::ref(jobOp),_1));
  boost::function1<void,string> fmutlStatus(boost::bind(&TMS_Data::ListJobsOptions::setMultipleStatus,boost::ref(jobOp),_1));
  boost::function1<void,long long> fworkId(boost::bind(&TMS_Data::ListJobsOptions::setWorkId,boost::ref(jobOp),_1));
  /*********** Out parameters *********************/
  TMS_Data::ListJobs job;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeListJobOp(argv[0],
      fjid,
      fnbCpu,
      fromDate,
      toDate,
      fown,
      stateStr,
      fpriority,
      fqueue,
      fmutlStatus,
      fworkId,
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

  CLICmd cmd = CLICmd (argc, argv, opt);

  // Process command
  try {

    // Parse the cli and setting the options found
    ret = cmd.parse(env_name_mapper());

    if (ret != CLI_SUCCESS) {
      helpUsage(*opt,"[options] machineId");
      return ret;
    }

    // PreProcess (adapt some parameters if necessary)
    checkVishnuConfig(*opt);
    if ( opt->count("help")) {
      helpUsage(*opt,"[options] machineId");
      return 0;
    }

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
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey = getLastSessionKey(getppid());

    // DIET call : submit
    if (!sessionKey.empty()) {
      printSessionKeyMessage();
      listJobs(sessionKey, machineId, job, jobOp);
    }
    if (jobOp.getOwner().size() == 0 && jobOp.getJobId().size() == 0  && jobOp.getNbCpu() <= 0
        && jobOp.getFromSubmitDate() <= 0 && jobOp.getToSubmitDate() <= 0 && !jobOp.isListAll()) {
      std::cout << job << std::endl;
    } else {
      displayListJobs(job);
    }
  } catch(VishnuException& e) {// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
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
