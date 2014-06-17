/**
 * \file submit_job.cpp
 * This file defines the VISHNU submit job command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include "GenericCli.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include "tmsUtils.hpp"

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName: The name of the command
 * \param setTypeFct: Function to set the type of the task
 * \param setMachineFct: Function to set the target machine option
 * \param setProgramNameFct: Function to set the job name option
 * \param setQueueFct: Function to set the queue option
 * \param setMemoryFct: Function to set the memory needed by the job
 * \param setNbCpuFct: Function to set the required number of cpu per node option
 * \param setNbNodeAndCpuFct: Function to set the required number of node and processor per node
 * \param setStdoutPathFct: Function to set the stdout path
 * \param setStderrPathFct: Function to set the stderr path
 * \param setMailNotificationFct: Function to set the type of the nofication (BEGIN, END, ERROR, and ALL)
 * \param setNotificationUserFct: Function to set the user that'll receive the email notification
 * \param setJobGroupFct: Function to set the job group
 * \param setWorkingDirFct: Function to set the job working directory
 * \param setCpuTimeFct: Function to set the cpu time limit of the job
 * \param setTextParamsFct: Function to set the textual parameters of the job
 * \param setFileParamsFct: Function to set the input file parameters of the job
 * \param setSpecificParamsFct: Function to set the possible specific parameters for the job
 * \param textParamsVector: The list of textual parameters
 * \param fileParamsVector: The list of file parameters
 * \param loadCriterionStr: The load value to use (for now three criterions are be used:
 *  minimum number of waiting jobs, minimum number of running jobs and the total number of job )
 * \param configFile: Represents the VISHNU config file
 * \param setWorkIdFct: Function to set the job work id
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeSubJobOp(string pgName,
             boost::function1<void, int>& setTypeFct,
             boost::function1<void, string>& setMachineFct,
             boost::function1<void, string>& setProgramNameFct,
             boost::function1<void, string>& setQueueFct,
             boost::function1<void, int>& setMemoryFct,
             boost::function1<void, int>& setNbCpuFct,
             boost::function1<void, string>& setNbNodeAndCpuFct,
             boost::function1<void, string>& setStdoutPathFct,
             boost::function1<void, string>& setStderrPathFct,
             boost::function1<void, string>& setMailNotificationFct,
             boost::function1<void, string>& setNotificationUserFct,
             boost::function1<void, string>& setJobGroupFct,
             boost::function1<void, string>& setWorkingDirFct,
             boost::function1<void, string>& setCpuTimeFct,
             boost::function1<void, string>& setTextParamsFct,
             boost::function1<void, string>& setFileParamsFct,
             boost::function1<void, string>& setSpecificParamsFct,
             vector<string>& textParamsVector,
             vector<string>& fileParamsVector,
             boost::function1<void, long long>& setWorkIdFct,
             string& loadCriterionStr,
             string& walltime,
             string& configFile)
{
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  // All cli options
  opt->add("type",
           "Sets the type of the task.",
           CONFIG,
           setTypeFct);
  opt->add("machine,r",
           "The id of the target machine. Default is autom.",
           CONFIG,
           setMachineFct);
  opt->add("name,n",
           "The name of the job",
           CONFIG,
           setProgramNameFct);
  opt->add("queue,q",
           "The queue to submit the job",
           CONFIG,
           setQueueFct);
  opt->add("wallTime,t",
           "The wall time for the job (in secondes or in the format [[HH:]MM:]SS)",
           CONFIG,
           walltime);
  opt->add("memory,m",
           "The memory needed by the job (in MegaBytes)",
           CONFIG,
           setMemoryFct);
  opt->add("nbCpu,P",
           "The number of cpu per node needed by the job",
           CONFIG,
           setNbCpuFct);
  opt->add("nbNodesAndCpuPerNode,N",
           "The number of node and processor per node",
           CONFIG,
           setNbNodeAndCpuFct);
  opt->add("outputPath,o",
           "The stdout path on the target machine",
           CONFIG,
           setStdoutPathFct);
  opt->add("errorPath,e",
           "The stderr path on the target machine",
           CONFIG,
           setStderrPathFct);

  opt->add("mailNotification,M",
           "Assigns the notification type of the job. Valid type values are "
           "BEGIN, END, ERROR, and ALL (any state change).",
           CONFIG,
           setMailNotificationFct);
  opt->add("mailNotifyUser,u",
           "The name of user to receive email notification of state changes as defined "
           "by the option mailNotification. The default value is the submitting user.",
           CONFIG,
           setNotificationUserFct);
  opt->add("group,g",
           "Assigns a job group name.",
           CONFIG,
           setJobGroupFct);
  opt->add("workingDir,D",
           "Assigns a job remote working dir",
           CONFIG,
           setWorkingDirFct);
  opt->add("cpuTime,T",
           "Assigns a job cpu limit time (in seconds or in the format [[HH:]MM:]SS)",
           CONFIG,
           setCpuTimeFct);
  opt->add("useLoad,L",
           "The criterion to automatically submit a job (for now three criterions are used:"
           " minimum number of waiting jobs, minimum number of running jobs and the total"
           "number of jobs). Predefined values are:\n"
           "0 or W: To select a machine that has minimum number of waiting jobs\n"
           "1 or T: To select a machine that has minimum number of total jobs (wainting and running)\n"
           "2 or R: To select a machine that has minimum number of running jobs",
           CONFIG,
           loadCriterionStr);
  opt->add("textParam,v",
           "Sets a textual parameter with a value passing in argument.\n"
           "E.g. --param PARAM1=value1. So at the execution, any environment variable named "
           "PARAM1 in the script will be set with 'value1'.\n\n"
           "SEE ALSO --textParams.",
           CONFIG,
           textParamsVector);
  opt->add("textParams,V",
           "Sets a list of space-separated textual parameters.\n"
           "E.g. --listParams=\"PARAM1=value1 PARAM2=value2.\n\n"
           "SEE ALSO --textParam.",
           CONFIG,
           setTextParamsFct);
  opt->add("specificParams,S",
           "Sets a list of space-separated textual parameters.\n"
           "E.g. --listParams=\"PARAM1=value1 PARAM2=value2.\n\n",
           CONFIG,
           setSpecificParamsFct);
  opt->add("fileParam,f",
           "Sets a local file as a file parameter for the script. This file will be uploaded onto the server "
           "before computing the script.\n"
           "E.g. fileParam PFILE1=/path/to/file will allow to upload the file located at '/path/to/file' "
           "onto the server.\n\n"
           "SEE ALSO --fileParams.",
           CONFIG,
           fileParamsVector);
  opt->add("fileParams,F",
           "Sets a list of local files as parameters for the script. These files will be  uploaded onto the server "
           "before computing the script.\n"
           "E.g. --fileParams=\"PFILE1=/path/to/file1 PFILE2=/path/to/file2\".\n\n"
           "SEE ALSO --fileParam.",
           CONFIG,
           setFileParamsFct);
  opt->add("workId,w",
           "Sets the identifier of the Work to which the job is related.  ",
           CONFIG,
           setWorkIdFct);

  return opt;
}

int main (int argc, char* argv[]){
  /******* Parsed value containers ****************/
  string configFile;
  string sessionKey;
  string scriptPath;
  string walltime;

  /********** EMF data ************/
  TMS_Data::SubmitOptions submitOptions;

  /******** Callback functions ******************/
  boost::function1<void,string> setProgramNameFct(boost::bind(&TMS_Data::SubmitOptions::setName,boost::ref(submitOptions),_1));
  boost::function1<void,int> setTypeFct(boost::bind(&TMS_Data::SubmitOptions::setType,boost::ref(submitOptions),_1));
  boost::function1<void,string> setMachineFct(boost::bind(&TMS_Data::SubmitOptions::setMachine,boost::ref(submitOptions),_1));
  boost::function1<void,string> setQueueFct(boost::bind(&TMS_Data::SubmitOptions::setQueue,boost::ref(submitOptions),_1));
  boost::function1<void,int> setMemoryFct(boost::bind(&TMS_Data::SubmitOptions::setMemory,boost::ref(submitOptions),_1));
  boost::function1<void,int> setNbCpuFct(boost::bind(&TMS_Data::SubmitOptions::setNbCpu,boost::ref(submitOptions),_1));
  boost::function1<void,string> setNbNodeAndCpuFct(boost::bind(&TMS_Data::SubmitOptions::setNbNodesAndCpuPerNode,boost::ref(submitOptions),_1));
  boost::function1<void,string> setStdoutPatchFct(boost::bind(&TMS_Data::SubmitOptions::setOutputPath,boost::ref(submitOptions),_1));
  boost::function1<void,string> setStderrPathFct(boost::bind(&TMS_Data::SubmitOptions::setErrorPath,boost::ref(submitOptions),_1));
  boost::function1<void,string> setMailNoticicationFct(boost::bind(&TMS_Data::SubmitOptions::setMailNotification,boost::ref(submitOptions),_1));
  boost::function1<void,string> setNotificationUserFct(boost::bind(&TMS_Data::SubmitOptions::setMailNotifyUser,boost::ref(submitOptions),_1));
  boost::function1<void,string> setUserGroupFct(boost::bind(&TMS_Data::SubmitOptions::setGroup,boost::ref(submitOptions),_1));
  boost::function1<void,string> setWorkingGroupFct(boost::bind(&TMS_Data::SubmitOptions::setWorkingDir,boost::ref(submitOptions),_1));
  boost::function1<void,string> serCpuTimeFct(boost::bind(&TMS_Data::SubmitOptions::setCpuTime,boost::ref(submitOptions),_1));
  boost::function1<void,string> setTextParamsFct(boost::bind(&TMS_Data::SubmitOptions::setTextParams,boost::ref(submitOptions),_1));
  boost::function1<void,string> setSpecificParamsFct(boost::bind(&TMS_Data::SubmitOptions::setSpecificParams,boost::ref(submitOptions),_1));
  boost::function1<void,string> setFileParamsFct(boost::bind(&TMS_Data::SubmitOptions::setFileParams,boost::ref(submitOptions),_1));
  boost::function1<void,long long> setWorkIdFct(boost::bind(&TMS_Data::SubmitOptions::setWorkId,boost::ref(submitOptions),_1));
  vector<string> textParamsVector ;
  vector<string> fileParamsVector ;
  std::string loadCriterionStr;
  /*********** Out parameters *********************/

  TMS_Data::Job job;
  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeSubJobOp(argv[0],
      setTypeFct,
      setMachineFct,
      setProgramNameFct,
      setQueueFct,
      setMemoryFct,
      setNbCpuFct,
      setNbNodeAndCpuFct,
      setStdoutPatchFct,
      setStderrPathFct,
      setMailNoticicationFct,
      setNotificationUserFct,
      setUserGroupFct,
      setWorkingGroupFct,
      serCpuTimeFct,
      setTextParamsFct,
      setFileParamsFct,
      setSpecificParamsFct,
      textParamsVector,
      fileParamsVector,
      setWorkIdFct,
      loadCriterionStr,
      walltime,
      configFile);

  opt->add("selectQueueAutom,Q",
           "allows to select automatically a queue which has the number of nodes requested by the user.",
           CONFIG);

  opt->add("scriptPath,z",
           "The script to execute",
           HIDDEN,
           scriptPath,
           1);
  opt->setPosition("scriptPath", 1);

  opt->add("posix,p",
           "is an option for submitting using the posix backend instead of the batch scheduler ",
           CONFIG);

  //process options and then the command
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  try {
    if (opt->count("posix")) {
      submitOptions.setPosix(true);
    }
    if (!walltime.empty()) {
      submitOptions.setWallTime(convertStringToWallTime(walltime));
    }

    //To set the load criterion
    if (loadCriterionStr.size() > 1) {
      std::cerr << "Invalid load criterion " + loadCriterionStr + "\n";
      return -1;
    }
    int loadCriterionType = NBWAITINGJOBS;
    if (loadCriterionStr.empty()) {
      switch(loadCriterionStr[0]) {
        case '2':
        case 'R':
          loadCriterionType = NBRUNNINGJOBS;
          break;
        case '1':
        case 'T':
          loadCriterionType = NBJOBS;
          break;
        case '0'://Default
        case 'W':
        default:
          loadCriterionType = NBWAITINGJOBS;
          break;
      }
    }
    TMS_Data::LoadCriterion_ptr loadCriterion =  new TMS_Data::LoadCriterion();
    loadCriterion->setLoadType(loadCriterionType);
    submitOptions.setCriterion(loadCriterion);

    if(opt->count("selectQueueAutom")) {
      submitOptions.setSelectQueueAutom(true);
    }

    //Validate textual parameters syntax, if any
    string paramOptName = "textParams" ;

    if(opt->count(paramOptName) || ! textParamsVector.empty()) {
      string paramStr ;
      int ret = vishnu::validateParameters(opt, paramStr, paramOptName, textParamsVector);
      if( ret != 0 ) return ret ;
      submitOptions.setTextParams(paramStr) ;
    }

    //Validate file parameters syntax, if any
    paramOptName = "fileParams" ;
    if(opt->count(paramOptName) || !fileParamsVector.empty()) {
      string paramStr ;
      int ret = vishnu::validateParameters(opt, paramStr, paramOptName, fileParamsVector) ;
      if( ret != 0 ) return ret ;
      submitOptions.setFileParams(paramStr) ;
    }

    // initializing VISHNU
    if (vishnuInitialize(const_cast<char*>(configFile.c_str()), argc, argv)) {
      errorUsage(argv[0], communicationErrorMsg, EXECERROR);
      return  CLI_ERROR_COMMUNICATION ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // vishnu call: submit
    if(! sessionKey.empty()){
      submitJob(sessionKey, scriptPath, job, submitOptions);
    }

    displaySubmit(job);

  } catch(VishnuException& except){// catch all Vishnu runtime error
    std::string  submitErrmsg = except.getMsg() + " [" + except.getMsgComp() + "]";
    errorUsage(bfs::basename(bfs::path(argv[0])), submitErrmsg, EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(except)){
      removeBadSessionKeyFromFile(getppid());
    }
    return except.getMsgI();
  } catch(std::exception& except){// catch all std runtime error
    errorUsage(argv[0],except.what());
    return CLI_ERROR_RUNTIME;
  }
  return 0;
}
