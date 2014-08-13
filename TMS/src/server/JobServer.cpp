/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>
#include "JobServer.hpp"
#include "TMSVishnuException.hpp"
#include "LocalAccountServer.hpp"
#include "SSHJobExec.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "DbFactory.hpp"
#include "ScriptGenConvertor.hpp"
#include "api_fms.hpp"
#include "utils.hpp"
#include "BatchFactory.hpp"
#include <pwd.h>
#include <cstdlib>
#include "Logger.hpp"
#include "ListJobServer.hpp"


/**
 * \brief Constructor
 * \param authKey The session info
 * \param machineId The machine identifier
 * \param sedConfig A pointer to the SeD configuration
 */
JobServer::JobServer(const std::string& authKey,
                     const std::string& machineId,
                     const ExecConfiguration_Ptr sedConfig)
  : mauthKey(authKey), mmachineId(machineId), msedConfig(sedConfig) {

  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();

  if (msedConfig) {
    std::string value;
    msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, value);
    mbatchType = vishnu::convertToBatchType(value);

    if (! isCloudBackend(mbatchType)) {
      msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, value);
      mbatchVersion = value;
    } else {
      mbatchVersion = "";
    }
  }
  if (! msedConfig->getConfigValue<int>(vishnu::STANDALONE, mstandaloneSed)) {
    mstandaloneSed = false;
  }
  checkMachineId(machineId);
  vishnu::validateAuthKey(mauthKey, mmachineId, mdatabase, muserSessionInfo);
}


/**
 * \brief Destructor
 */
JobServer::~JobServer() { }

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options a json object describing options
 * \param defaultBatchOption The default batch options
 * \return The resulting job ID. Raises an exception on error
 */
std::string
JobServer::submitJob(std::string& scriptContent,
                     JsonObject* options,
                     const std::vector<std::string>& defaultBatchOption)
{

  LOG("[INFO] Job received", LogInfo);

  TMS_Data::Job jobInfo;

  if (scriptContent.empty()) {
    throw UserException(ERRCODE_INVALID_PARAM, "Empty script content");
  }

  try {
    int usePosix = options->getIntProperty("posix");
    if (usePosix != JsonObject::UNDEFINED_PROPERTY && usePosix != 0) {
      mbatchType = POSIX;
    }

    jobInfo.setWorkId(options->getIntProperty("scriptpath", 0));
    jobInfo.setWorkId(options->getIntProperty("workid", 0));
    setRealFilePaths(scriptContent, options, jobInfo);
    jobInfo.setMachine(mmachineId);
    jobInfo.setStatus(vishnu::STATE_UNDEFINED);

    // the way of setting job owner varies from classical batch scheduler to cloud backend
    switch (mbatchType) {
      case OPENNEBULA:
      case DELTACLOUD:
        //FIXME: does not work with db model refactoring
        jobInfo.setLocalAccount( vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], true, "root") );
        break;
      default:
        jobInfo.setLocalAccount(vishnu::convertToString(muserSessionInfo.num_user));
        break;
    }

    exportJobEnvironments(jobInfo);

    if (mstandaloneSed != 0) {
      handleNativeBatchExec(SubmitBatchAction,
                            createJobScriptExecutaleFile(scriptContent, options, defaultBatchOption),
                            options,
                            jobInfo,
                            mbatchType,
                            mbatchVersion);
    } else {
      handleSshBatchExec(SubmitBatchAction,
                         createJobScriptExecutaleFile(scriptContent, options, defaultBatchOption),
                         options,
                         jobInfo,
                         mbatchType,
                         mbatchVersion);
    }
  } catch (VishnuException& ex) {
    jobInfo.setSubmitError(ex.what());
    jobInfo.setErrorPath("");
    jobInfo.setOutputPath("");
    jobInfo.setOutputDir("");
    jobInfo.setStatus(vishnu::STATE_FAILED);
    dbSave(SubmitBatchAction, jobInfo);
    throw;
  }
  return jobInfo.getId();
}

/**
 * @brief Submit job using ssh mechanism
 * @param action action The type of action (cancel, submit...)
 * @param scriptPath The path of the script to executed
 * @param baseJobInfo The base job info
 * @param options: an object containing options
 * @param batchType The batch type
 * @param batchVersion The batch version
*/
void
JobServer::handleSshBatchExec(int action,
                              const std::string& scriptPath,
                              JsonObject* options,
                              TMS_Data::Job& baseJobInfo,
                              int batchType,
                              const std::string& batchVersion)
{
  SSHJobExec sshJobExec(muserSessionInfo.user_aclogin,
                        muserSessionInfo.machine_address,
                        static_cast<BatchType>(batchType),
                        batchVersion, // ignored for POSIX backend
                        JsonObject::serialize(baseJobInfo),
                        options->encode());

  sshJobExec.setDebugLevel(mdebugLevel);
  TMS_Data::ListJobs jobSteps;

  switch(action) {
    case SubmitBatchAction:
      sshJobExec.sshexec("SUBMIT", scriptPath, jobSteps);
      // Submission with deltacloud and opennebula doesn't make copy of the script
      // So the script needs to be kept until the end of the execution
      // Clean the temporary script if not deltacloud
      if (! isCloudBackend(mbatchType) && mdebugLevel) {
        vishnu::deleteFile(scriptPath.c_str());
      }
      saveJobSteps(jobSteps, baseJobInfo);
      break;
    case CancelBatchAction:
      sshJobExec.sshexec("CANCEL", "", jobSteps);
      dbSave(action, *(jobSteps.getJobs().get(0)));
      break;
    default:
      throw TMSVishnuException(ERRCODE_INVALID_PARAM, "unknown batch action");
      break;
  }

}

/**
 * @brief Submit job using ssh mechanism
 * @param action action The type of action (cancel, submit...)
 * @param scriptPath The path of the script to executed
 * @param options: an object containing options
 * @param jobInfo The default information provided to the job
 * @param batchType The batch type. Ignored for POSIX backend
 * @param batchVersion The batch version. Ignored for POSIX backend
*/
void
JobServer::handleNativeBatchExec(int action,
                                 const std::string& scriptPath,
                                 JsonObject* options,
                                 TMS_Data::Job& jobInfo,
                                 int batchType,
                                 const std::string& batchVersion)
{

  int ipcPipe[2];
  char ipcMsgBuffer[255];

  if (pipe(ipcPipe) != 0)	 {	/* Create communication pipe*/
    throw TMSVishnuException(ERRCODE_RUNTIME_ERROR, "Pipe creation failed");
  }

  pid_t pid = fork();

  if (pid < 0) {
    throw TMSVishnuException(ERRCODE_RUNTIME_ERROR, "Fork failed");
  }

  const std::string SUCCESS_MSG_PREFIX = "SYSFERADS_SUCCESS";
  int processExitCode = 0;
  std::string errorMsg = "FAILED";
  if (pid == 0)  /** Child process */ {
    close(ipcPipe[0]);
    processExitCode = 0;

    // make a copy of the job passed as reference to avoid conflict
    TMS_Data::Job jobCopy = jobInfo;

    // if not cloud-mode submission, switch user before running the request
    if (! isCloudBackend(mbatchType)) {
      processExitCode = setuid(getSystemUid(muserSessionInfo.user_aclogin));
      if (processExitCode != 0) { // write error message to pipe for the parent
        errorMsg = std::string(strerror(errno));
        write(ipcPipe[1], errorMsg.c_str(), errorMsg.size());
        exit(processExitCode);
      }
    }

    BatchServer* batchServer = NULL;
    try {
      batchServer = BatchServer::getBatchServer(batchType, batchVersion);
      processExitCode = -1;

      switch (action) {

        // HANDLE SUBMIT
        case SubmitBatchAction: {
          // create output dir if needed
          if (! jobCopy.getOutputDir().empty()) {
            vishnu::createDir(jobCopy.getOutputDir());
          }
          // submit the job
          TMS_Data::ListJobs jobSteps;
          batchServer->submit(vishnu::copyFileToUserHome(scriptPath), options->getSubmitOptions(), jobSteps, NULL);
          saveJobSteps(jobSteps, jobCopy);

          // set exit success message
          errorMsg = boost::str(boost::format("%1%:%2%") % SUCCESS_MSG_PREFIX % jobCopy.getId());
        }
          break;

          // HANDLE CANCEL
        case CancelBatchAction:
          if (isCloudBackend(mbatchType) ){
            batchServer->cancel(jobCopy.getVmId()) ;
          } else {
            batchServer->cancel(jobCopy.getBatchJobId());
          }
          jobCopy.setStatus(vishnu::STATE_CANCELLED);
          dbSave(action, jobCopy);

          // set exit success message
          errorMsg = boost::str(boost::format("%1%:%2%") % SUCCESS_MSG_PREFIX % jobCopy.getId());
          break;
        default:
          throw TMSVishnuException(ERRCODE_INVALID_PARAM, "Unknown batch action");
          break;
      }
    } catch (const VishnuException & ex) {
      errorMsg = std::string(ex.what());
      LOG("[ERROR] "+ errorMsg, LogErr);
    }
    if (batchServer) {
      delete batchServer;
    }
    write(ipcPipe[1], errorMsg.c_str(), errorMsg.size());
    exit(processExitCode);
  } else { /** Parent process*/ // wait that child exists
    close(ipcPipe[1]);
    int exitCode;
    waitpid(pid, &exitCode, 0);

    // get possible error message send by the child
    size_t nbRead = read(ipcPipe[0], ipcMsgBuffer, 255);
    errorMsg = std::string(ipcMsgBuffer, nbRead);

    if (errorMsg.find(SUCCESS_MSG_PREFIX) == std::string::npos) {
      throw TMSVishnuException(ERRCODE_RUNTIME_ERROR, boost::str(boost::format("Job worker exited on error: %1%") % errorMsg));
    } else {
      // on success errorMsg has should be SUCCESS_MSG_PREFIX:data. here data correspond to job id
      size_t dataStartPos = errorMsg.find(":");
      jobInfo.setId( errorMsg.substr(dataStartPos+1, std::string::npos) );
    }
  }
}

/**
      * \brief Function to treat the default submission options
      * \param scriptOptions The list of the option value
      * \param cmdsOptions The list of the option value
      * \return raises an exception on error
    */
void
JobServer::processDefaultOptions(const std::vector<std::string>& defaultBatchOption,
                                 std::string& content, std::string& key) {
  size_t position = 0;
  std::string key1;
  int count = 0;
  int countOptions = defaultBatchOption.size();
  while (count < countOptions) {
    key1 =  defaultBatchOption.at(count);
    position = 0;
    int found =0;
    while (position != std::string::npos && ! found) {
      position = content.find(key.c_str(), position);
      if (position != std::string::npos) {
        size_t pos1 = content.find("\n", position);
        std::string line = content.substr(position, pos1-position);
        position++;
        size_t pos2 = line.find(key1.c_str());
        if (pos2 != std::string::npos) {
          found =1;
          break;
        }
      }
    }
    if (!found) {
      std::string lineoption = key + " " + defaultBatchOption.at(count) + " " + defaultBatchOption.at(count +1) + "\n";
      insertOptionLine(lineoption, content, key);
    }
    count +=2;
  }
}
/**
      * \brief Function to insert option into string
      * \param optionLineToInsert the option to insert
      * \param content The buffer containing the inserted option
      * \return raises an exception on error
    */
void
JobServer::insertOptionLine(std::string& optionLineToInsert,
                            std::string& content, std::string& key) {
  size_t pos = 0;
  size_t posLastDirective = 0;

  while (pos!=std::string::npos) {
    pos = content.find(key.c_str(), pos);
    if (pos!=std::string::npos) {
      size_t pos1 = 0;
      pos1 = content.find("\n", pos);
      while (content.compare(pos1-1,1,"\\") == 0) {
        pos1 = content.find("\n", pos1 + 1);
      }
      std::string line = content.substr(pos, pos1-pos);
      if(content.compare(pos-1,1,"\n")==0) {
        if(mbatchType==LOADLEVELER) {
          std::string line_tolower(line);
          std::transform(line.begin(), line.end(), line_tolower.begin(), ::tolower);
          if(line_tolower.find("queue")!=std::string::npos) {
            break;
          }
        }
        posLastDirective = pos + line.size() + 1;
      }
      pos++;
    }
  }
  content.insert(posLastDirective, optionLineToInsert);
}

/**
      * \brief Function to cancel job
      * \param options Object containing options
      * \return raises an exception on error
    */
int JobServer::cancelJob(JsonObject* options)
{

  std::string jobId = options->getStringProperty("jobid");
  std::string userId = options->getStringProperty("user");

  ListJobServer jobLister(mauthKey);
  TMS_Data::ListJobsOptions jobListerOptions;
  jobListerOptions.setMachineId(mmachineId);

  // set options for listing related jobs
  bool useJobIdOption = false;

  if (isAdminSession()) {
    if (! userId.empty()) {
      if (userId == ALL_KEYWORD) {
        jobListerOptions.setListAll(true);
      } else {
        jobListerOptions.setOwner(userId);
      }
    } else {
      useJobIdOption = true;
    }
  } else {
    if (! userId.empty()) {
      throw TMSVishnuException(ERRCODE_PERMISSION_DENIED, "Only privileged users can specify user option");
    }
    useJobIdOption = true;
  }

  if (useJobIdOption) {
    if (jobId.empty()) {
      throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, "No job specify for cancel");
    }
    if (jobId == ALL_KEYWORD) {
      jobListerOptions.setOwner(userId);
    } else {
      jobListerOptions.setJobId(jobId);
    }
  }

  // now list jobs
  boost::scoped_ptr<TMS_Data::ListJobs> jobList( jobLister.list(&jobListerOptions) );

  // cancel resulting jobs
  for (int index = 0; index < jobList->getNbJobs(); ++index) {
    if (mstandaloneSed != 0) {
      handleNativeBatchExec(CancelBatchAction, "",
                            options,
                            *(jobList->getJobs().get(index)),
                            mbatchType,
                            mbatchVersion);
    } else {
      handleSshBatchExec(CancelBatchAction,
                         "",
                         options,
                         *(jobList->getJobs().get(index)),
                         mbatchType,
                         mbatchVersion);
    }
  }

  return 0;
}


/**
      * \brief Function to get job information
      * \param jobId The id of the job
      * \return The job data structure
    */
TMS_Data::Job
JobServer::getJobInfo(const std::string& jobId)
{
  ListJobServer jobLister(mauthKey);
  TMS_Data::ListJobsOptions jobListerOptions;

  jobListerOptions.setJobId(jobId);
  boost::scoped_ptr<TMS_Data::ListJobs> jobList( jobLister.list(&jobListerOptions) );

  if (jobList->getNbJobs() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID,jobId);
  }

  TMS_Data::Job job = *(jobList->getJobs().at(0));

  return job;
}


/**
      * \brief Function to scan VISHNU error message
      * \param errorInfo the error information to scan
      * \param code The code The code of the error
      * \param message The message associeted to the error code
      * \return raises an exception on erroor
    */
void JobServer::scanErrorMessage(const std::string& errorInfo, int& code, std::string& message) {

  code = ERRCODE_INVEXCEP;

  size_t pos = errorInfo.find('#');
  if (pos!=std::string::npos) {
    std::string codeInString = errorInfo.substr(0,pos);
    if (! codeInString.empty()) {
      try {
        code = boost::lexical_cast<int>(codeInString);
      } catch(boost::bad_lexical_cast &) {
        code = ERRCODE_INVEXCEP;
      }
      message = errorInfo.substr(pos+1);
    }
  }
}

/**
      * \brief Function to convert a given date into correspondant long value
      * \param date The date to convert
      * \return The converted value
    */
long long JobServer::convertToTimeType(std::string date) {

  if (date.empty() || date.find("0000-00-00") != std::string::npos) { // For mysql empty date is 0000-00-00, not empty
    return 0;
  }

  boost::posix_time::ptime pt(boost::posix_time::time_from_string(date));
  boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
  boost::posix_time::time_duration::sec_type time = (pt - epoch).total_seconds();

  return (long long) time_t(time);
}

/**
      * \brief To get the main configuration
      * \return the pointer to configuration object
    */
ExecConfiguration_Ptr
JobServer::getSedConfig() const {
  return msedConfig;
}

/*
      * \brief Return the directive associated to the batch scheduler
      * \param seperator Hold the seperator used to define parameter
    */
std::string JobServer::getBatchDirective(std::string& seperator) const {

  seperator =  " ";
  std::string directive = "";
  switch(mbatchType) {
    case TORQUE :
      directive = "#PBS";
      break;
    case LOADLEVELER :
      directive = "# @";
      seperator = " = ";
      break;
    case SLURM :
      directive = "#SBATCH";
      break;
    case LSF :
      directive = "#BSUB";
      break;
    case SGE :
      directive = "#$";
      break;
    case PBSPRO :
      directive = "#PBS";
      break;
    case POSIX :
      directive = "#%";
      break;
    case DELTACLOUD : // return default ""
    default :
      break;
  }
  return directive;
}

/**
      * \brief Set specific parameters for job submission
      * \param specificParamss The string containing the list of parameters
      * \param scriptContent The content of the script when required
    */
void JobServer::handleSpecificParams(const std::string& specificParams,
                                     std::string& scriptContent) {

  std::string sep = " ";
  std::string directive = getBatchDirective(sep);
  size_t pos1 = 0;
  size_t pos2 = 0;
  std::string& params = const_cast<std::string&>(specificParams);
  pos2 = params.find("=");
  while (pos2 != std::string::npos) {
    size_t pos3 = 0;
    pos3 = params.find(" ");
    if(pos3 != std::string::npos) {
      std::string lineoption = directive +" "+ params.substr(pos1, pos2-pos1) + sep +  params.substr(pos2+1, pos3-pos2-1) + "\n";
      insertOptionLine(lineoption, scriptContent, directive);
      params.erase(0, pos3);
      boost::algorithm::trim_left(params);
    } else {
      std::string lineoption = directive +" "+ params.substr(pos1, pos2-pos1)+ sep +  params.substr(pos2+1, params.size()-pos2) + "\n";
      insertOptionLine(lineoption, scriptContent, directive);
      break;
    }
    pos2 = params.find("=");
  }
}

/**
      * \brief Function to set the Working Directory
      * \param scriptContent The script content
      * \param options a json object describing options
      * \param jobInfo The job information, could be altered with output path
    */
void
JobServer::setRealFilePaths(std::string& scriptContent,
                            JsonObject* options,
                            TMS_Data::Job& jobInfo)
{
  std::string workingDir = muserSessionInfo.user_achome;
  std::string scriptPath = "";
  std::string inputDir = "";

  if (isCloudBackend(mbatchType)) {
    std::string mountPoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], true);
    if (mountPoint.empty()) {
      workingDir = boost::str(boost::format("/tmp/%1%")
                              % vishnu::generatedUniquePatternFromCurTime(jobInfo.getId()));
    } else {
      workingDir = boost::str(boost::format("%1%/%2%")
                              % mountPoint
                              % vishnu::generatedUniquePatternFromCurTime(jobInfo.getId()));
    }

    inputDir =  boost::str(boost::format("%1%/INPUT") % workingDir);
    scriptPath = boost::str(boost::format("%1%/vishnu-job-script-%2%%3%")
                            % inputDir
                            % jobInfo.getId()
                            % bfs::unique_path("%%%%%%").string());

    vishnu::createDir(workingDir, true);
    vishnu::createDir(inputDir, true);

    std::string directory = "";
    try {
      directory = vishnu::moveFileData(options->getStringProperty("fileparams"), inputDir);
    } catch(bfs::filesystem_error &ex) {
      throw SystemException(ERRCODE_RUNTIME_ERROR, ex.what());
    }

    if(directory.length() > 0) {
      std::string fileparams = options->getStringProperty("fileparams");
      vishnu::replaceAllOccurences(fileparams, directory, inputDir);
      options->setProperty("fileparams", fileparams);
    }
  } else {
    std::string path = options->getStringProperty("workingdir");
    if (! path.empty()) {
      workingDir = path;
    } else {
      options->setProperty("workingdir", workingDir);
    }
    scriptPath = boost::str(boost::format("/tmp/vishnuJobScript%1%-%2%")
                            % bfs::unique_path("%%%%%%").string()
                            % jobInfo.getId());
  }

  if (isCloudBackend(mbatchType) || scriptContent.find("VISHNU_OUTPUT_DIR") != std::string::npos) {
    std::string outputDir = boost::str(boost::format("%1%/VISHNU_OUTPUT_DIR_%2%")
                                       % workingDir
                                       % vishnu::generatedUniquePatternFromCurTime(jobInfo.getId()));
    vishnu::replaceAllOccurences(scriptContent, "$VISHNU_OUTPUT_DIR", outputDir);
    vishnu::replaceAllOccurences(scriptContent, "${VISHNU_OUTPUT_DIR}", outputDir);
    jobInfo.setOutputDir(outputDir);
  } else {
    jobInfo.setOutputDir("");
  }

  jobInfo.setWorkingDir(workingDir);
  options->setProperty("scriptpath", scriptPath);
}


/**
     * \brief Function to process the script with options
     * \param content The script content
     * \param options the options to submit job
     * \param defaultBatchOption The default batch options
     * \return the processed script content
    */
std::string
JobServer::processScript(std::string& content,
                         JsonObject* options,
                         const std::vector<std::string>& defaultBatchOption,
                         const std::string& machineName)
{
  std::string convertedScript;

  vishnu::replaceAllOccurences(content, "$VISHNU_SUBMIT_MACHINE_NAME", machineName);
  vishnu::replaceAllOccurences(content, "${VISHNU_SUBMIT_MACHINE_NAME}", machineName);

  std::string currentOption = options->getStringProperty("textparams");
  if (! currentOption.empty()) {
    vishnu::setParams(content, currentOption);
  }
  currentOption = options->getStringProperty("fileparams");
  if (! currentOption.empty()) {
    vishnu::setParams(content, currentOption) ;
  }
  boost::shared_ptr<ScriptGenConvertor> scriptConvertor(vishnuScriptGenConvertor(mbatchType, content));
  if(scriptConvertor->scriptIsGeneric()) {
    std::string genScript = scriptConvertor->getConvertedScript();
    convertedScript = genScript;
  } else {
    convertedScript = content;
  }
  std::string sep = " ";
  std::string directive = getBatchDirective(sep);
  currentOption = options->getStringProperty("specificparams");
  if (! currentOption.empty()) {
    handleSpecificParams(currentOption, convertedScript);
  }

  if (! defaultBatchOption.empty()){
    processDefaultOptions(defaultBatchOption, convertedScript, directive);
  }
  if ( isCloudBackend(mbatchType) ) {
    const std::string NODE_FILE = boost::str(boost::format("%1%/NODEFILE") % options->getStringProperty("outputdir"));
    vishnu::replaceAllOccurences(content, "$VISHNU_BATCHJOB_NODEFILE", NODE_FILE);
    vishnu::replaceAllOccurences(content, "${VISHNU_BATCHJOB_NODEFILE}", NODE_FILE);
  }
  return convertedScript;
}


/**
     * @brief Update the result job steps with the base information of the job and save them
     * @param jobSteps The list of steps
     * @param baseJobInfo The base job info
     */
void
JobServer::saveJobSteps(TMS_Data::ListJobs& jobSteps, TMS_Data::Job& baseJobInfo)
{
  std::vector<std::string> stepIds;
  int nbSteps = jobSteps.getJobs().size();
  for (int step = 0; step < nbSteps; ++step) {
    TMS_Data::Job_ptr currentJobPtr = jobSteps.getJobs().get(step);
    currentJobPtr->setMachine(baseJobInfo.getMachine());
    currentJobPtr->setWorkId(baseJobInfo.getWorkId());
    currentJobPtr->setPath(baseJobInfo.getPath());
    currentJobPtr->setLocalAccount(baseJobInfo.getLocalAccount());
    currentJobPtr->setOutputDir(baseJobInfo.getOutputDir());
    currentJobPtr->setWorkingDir(baseJobInfo.getWorkingDir());
    dbSave(SubmitBatchAction, *currentJobPtr);
    stepIds.push_back(currentJobPtr->getId());
  }

  // now each job's compute related steps if applicable
  for (int step = 0; step < nbSteps; ++step) {
    std::string relatedStepsList = "";
    for (int otherStep = 0; otherStep < nbSteps; ++otherStep) {
      if (otherStep != step) {
        if (! relatedStepsList.empty()) {
          relatedStepsList.append(",");
        }
        relatedStepsList.append(jobSteps.getJobs().get(otherStep)->getId());
      }
    }
    mdatabase->process(boost::str(boost::format("UPDATE job"
                                                " SET relatedSteps='%1%'"
                                                " WHERE job.id=%2%")
                                  % relatedStepsList
                                  % stepIds[step]));
  }
  if (! stepIds.empty()) {
    baseJobInfo.setId(stepIds.at(0));
  }
}


/**
     * @brief Get the uid corresponding to given system user name
     * @param username
     * @return
     */
uid_t
JobServer::getSystemUid(const std::string& name)
{
  passwd* info = getpwnam(name.c_str());
  if (! info) {
    throw TMSVishnuException(ERRCODE_INVALID_PARAM,
                             "The user doesn't have a valid uid "+name);
  }
  return info->pw_uid;
}


void
JobServer::checkMachineId(std::string machineId) {
  std::string sqlMachineRequest = boost::str(
                                    boost::format("SELECT machineid"
                                                  " FROM machine"
                                                  " WHERE machineid='%1%'"
                                                  " AND status<>%2%")%mdatabase->escapeData(machineId) %vishnu::STATUS_DELETED);
  boost::scoped_ptr<DatabaseResult> machine(mdatabase->getResult(sqlMachineRequest.c_str()));
  if(machine->getNbTuples()==0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
  }
}


/**
       * @brief create the executable for submit job script
       * @param content The script content
       * @param options The submit options
       * @param defaultBatchOption The default batch options
       * @return The script path
     */
std::string
JobServer::createJobScriptExecutaleFile(std::string& content,
                                        JsonObject* options,
                                        const std::vector<std::string>& defaultBatchOption)
{
  std::string path = options->getStringProperty("scriptpath");

  // Create the file on the file system
  vishnu::saveInFile(path, processScript(content,
                                         options,
                                         defaultBatchOption,
                                         muserSessionInfo.machine_address));

  vishnu::makeFileExecutable(path);

  return path;
}

/**
     * @brief Export environment variables used throughout the execution, notably in cloud mode
     * @param defaultJobInfo The default job info
     */
void
JobServer::exportJobEnvironments(const TMS_Data::Job& defaultJobInfo)
{
  setenv("VISHNU_JOB_ID", defaultJobInfo.getId().c_str(), 1);
  setenv("VISHNU_OUTPUT_DIR", defaultJobInfo.getOutputDir().c_str(), 1);
}


/**
     * \brief Function to save the encapsulated job into the database
     * @param action The type of action to finalize (submit, cancel...)
     * @param job The concerned job
     */
void
JobServer::dbSave(int action, TMS_Data::Job& job)
{
  if (action == CancelBatchAction) {
    std::string query = boost::str(
                          boost::format("UPDATE job"
                                        " SET status=%1%"
                                        " WHERE job.id='%2%';")
                          % vishnu::convertToString(job.getStatus())
                          % job.getId());
    mdatabase->process(query);
    LOG(boost::str(boost::format("[INFO] job cancelled: %1%") % job.getId()), LogInfo);

  } else if (action == SubmitBatchAction) {
    // Append the machine name to the error and output path if necessary
    size_t pos = job.getOutputPath().find(":");
    std::string prefixOutputPath = (pos == std::string::npos)? muserSessionInfo.machine_address+":" : "";
    job.setOutputPath(prefixOutputPath+job.getOutputPath());
    pos = job.getErrorPath().find(":");
    std::string prefixErrorPath = (pos == std::string::npos)? muserSessionInfo.machine_address+":" : "";
    job.setErrorPath(prefixErrorPath+job.getErrorPath());

    std::string query = boost::str(
                          boost::format("INSERT INTO job"
                                        "(name, batchType, batchJobId, scriptpath, outputPath, errorPath, outputDir,"
                                        "nbCpus, workingdir, status, submitDate, queue, groupName, description,"
                                        "memLimit, nbNodes, nbNodesAndCpuPerNode, wallClockLimit, priority,"
                                        "work_id, relatedSteps, vmId, vmIp,"
                                        "vsession_numsessionid, users_numuserid, machine_nummachineid)"
                                        "VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%', '%7%',"
                                        "'%8%', '%9%','%10%','%11%','%12%','%13%','%14%',"
                                        "'%15%', '%16%', '%17%', '%18%',%19%,"
                                        "%20%, '%21%', '%22%','%23%',"
                                        " %24%, %25%, %26%)")
                          % job.getName()
                          % mbatchType
                          % job.getBatchJobId()
                          % job.getPath()
                          % job.getOutputPath()
                          % job.getErrorPath()
                          % job.getOutputDir()
                          % job.getNbCpus()
                          % job.getWorkingDir()
                          % job.getStatus()
                          % "CURRENT_TIMESTAMP" //job.getSubmitDate()
                          % job.getQueue()
                          % job.getGroupName()
                          % job.getDescription()
                          % job.getMemLimit()
                          % job.getNbNodes()
                          % job.getNbNodesAndCpuPerNode()
                          % job.getWallClockLimit()
                          % job.getPriority()
                          % (job.getWorkId() > 0 ? vishnu::convertToString(job.getWorkId()) : "NULL")
                          % job.getRelatedSteps()
                          % job.getVmId()
                          % job.getVmIp()
                          % muserSessionInfo.num_session
                          % muserSessionInfo.num_user
                          % muserSessionInfo.num_machine
                          );

    std::pair<int, uint64_t>
        result = mdatabase->process(query);

    job.setId( vishnu::convertToString(result.second) );

    // logging
    if (job.getSubmitError().empty()) {
      LOG(boost::str(
            boost::format("[INFO] job submitted: %1%. User: %2%. Owner: %3%")
            % job.getId()
            % muserSessionInfo.userid
            % muserSessionInfo.user_aclogin), LogInfo);
    } else {
      LOG((boost::str(
             boost::format("[WARN] submission error: %1% [%2%]")
             % job.getId()
             % job.getSubmitError())), LogWarning);
    }
  } else {
    throw TMSVishnuException(ERRCODE_INVALID_PARAM, "unknown batch action");
  }
}

