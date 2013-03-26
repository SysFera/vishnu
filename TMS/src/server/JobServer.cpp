/**
 * \file JobServer.cpp
 * \brief This file contains the VISHNU JobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#include "JobServer.hpp"
#include "TMSVishnuException.hpp"
#include "LocalAccountServer.hpp"
#include "UserServer.hpp"
#include "SSHJobExec.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "DbFactory.hpp"
#include "ScriptGenConvertor.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format/format_class.hpp>

#include <fcntl.h>
#include "api_fms.hpp"
#include "sys/stat.h"

using namespace std;
using namespace vishnu;

/**
 * \param sessionServer The object which encapsulates the session information
 * \param machineId The machine identifier
 * \param job The job data structure
 * \param sedConfig A pointer to the SeD configuration
 * \brief Constructor
 */
JobServer::JobServer(const SessionServer& sessionServer,
                     const std::string& machineId,
                     const TMS_Data::Job& job,
                     const ExecConfiguration_Ptr sedConfig):
  msessionServer(sessionServer), mmachineId(machineId), mjob(job), msedConfig(sedConfig) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  if (msedConfig) {
    std::string value;
    msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, value);
    mbatchType = vishnu::convertToBatchType(value);
    if (mbatchType != DELTACLOUD) {
      msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, value);
      mbatchVersion = value;
    } else {
      mbatchVersion = "";
    }
  }
}

/**
 * \brief Destructor
 */
JobServer::~JobServer() { }

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \param vishnuId The VISHNU identifier
 * \param defaultBatchOption The default batch options
 * \return raises an exception on error
 */
int JobServer::submitJob(std::string& scriptContent,
                         TMS_Data::SubmitOptions& options,
                         const int& vishnuId,
                         const std::vector<std::string>& defaultBatchOption)
{
  bool succeed = false;
  int errCode = ERRCODE_RUNTIME_ERROR;
  std::string errMsg  = "";
  try {
    msessionServer.check(); //To check the sessionKey
    std::string sessionKey = (msessionServer.getData()).getSessionKey();
    std::string sessionId = msessionServer.getAttribut("where sessionkey='"+sessionKey+"'", "numsessionid");
    mjob.setSessionId(sessionId);
    std::string vishnuJobId = vishnu::getObjectId(vishnuId, "formatidjob", JOB, mmachineId);
    mjob.setJobId(vishnuJobId);
    mjob.setStatus(vishnu::STATE_UNDEFINED);
    mjob.setWorkId(options.getWorkId());
    mjob.setOutputDir("");
    std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);

    if (options.isPosix()) {
      mbatchType = POSIX;
    }
    std::string suffix = mjob.getJobId()+vishnu::createSuffixFromCurTime();
    std::string scriptPath = computeWorkingDir(options,suffix);
    if (scriptContent.find("VISHNU_OUTPUT_DIR") != std::string::npos || mbatchType == DELTACLOUD ) {
      computeOutputDir(options.getWorkingDir(), suffix, scriptContent);
    }
    ::ecorecpp::serializer::serializer optSer;
    ::ecorecpp::serializer::serializer jobSer;
    std::string submitOptionsSerialized = optSer.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options));
    std::string jobSerialized =  jobSer.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));
    std::string machineName =  getMachineName(mmachineId);
    SSHJobExec sshJobExec(acLogin, machineName,
                          mbatchType,
                          mbatchVersion, // it will work for POSIX at the POSIX backend ignores the batch version
                          jobSerialized, submitOptionsSerialized);
    sshJobExec.setDebugLevel(mdebugLevel);  // Set the debug level
    std::string convertedScript = processScript(scriptContent,options, defaultBatchOption, machineName);
    vishnu::saveInFile(scriptPath, convertedScript);
    if(0 != chmod(scriptPath.c_str(),
                  S_IRUSR|S_IXUSR|
                  S_IRGRP|S_IXGRP|
                  S_IROTH|S_IXOTH)) {
      throw SystemException(ERRCODE_INVDATA, "Unable to make the script executable" + scriptPath) ;
    }
    sshJobExec.sshexec("SUBMIT", std::string(scriptPath)); // Submit the job
    // Submission with deltacloud doesn't make copy of the script
    // So the script needs to be kept until the end of the execution
    // Clean the temporary script if not deltacloud
    if (mbatchType != DELTACLOUD && mdebugLevel) {
      vishnu::deleteFile(scriptPath.c_str());
    }
    std::string errorInfo = sshJobExec.getErrorInfo(); // Check if some errors occured during the submission
    if (!errorInfo.empty()) {
      int code;
      std::string message;
      scanErrorMessage(errorInfo, code, message);
      throw TMSVishnuException(code, message);
    }
    deserializeJob(sshJobExec.getJobSerialized());
    mjob.setSubmitMachineId(mmachineId);
    mjob.setSubmitMachineName(machineName);

    string scriptContentStr = std::string(convertedScript);
    size_t pos = scriptContentStr.find("'");
    while (pos!=std::string::npos) {
      scriptContent.replace(pos, 1, " ");
      pos = scriptContent.find("'");
    }
    // Set the job owner for SGE and Deltacloud
    // For other batch schedulers this information is known
    if(mbatchType == SGE || mbatchType == DELTACLOUD || mbatchType == POSIX) {
      mjob.setOwner(acLogin);
    }
    pos = mjob.getOutputPath().find(":");
    std::string prefixOutputPath = (pos == std::string::npos)? mjob.getSubmitMachineName()+":" : "";
    mjob.setOutputPath(prefixOutputPath+mjob.getOutputPath());
    pos = mjob.getErrorPath().find(":");
    std::string prefixErrorPath = (pos == std::string::npos)? mjob.getSubmitMachineName()+":" : "";
    mjob.setErrorPath(prefixErrorPath+mjob.getErrorPath());
    succeed = true;
  } catch (VishnuException& ex) {
    succeed = false;
    scanErrorMessage(ex.buildExceptionString(), errCode, errMsg);
    boost::erase_all(errMsg, "'");
    boost::erase_all(errMsg, "\"");
    mjob.setErrorPath(errMsg);
    mjob.setOutputPath("");
    mjob.setOutputDir("");
    mjob.setStatus(vishnu::STATE_FAILED);
  }
  try {
    recordJob2db();
  } catch (VishnuException& ex) {
    succeed = false;
    scanErrorMessage(ex.buildExceptionString(), errCode, errMsg);
  }

  if (!succeed) {
    throw TMSVishnuException(errCode, mjob.getJobId()+": "+errMsg);
  }
  return 0;
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
    while (position!=string::npos && !found) {
      position = content.find(key.c_str(), position);
      if(position!=std::string::npos) {
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
JobServer::insertOptionLine( std::string& optionLineToInsert,
                             std::string& content, std::string& key) {
  size_t pos = 0;
  size_t posLastDirective = 0;

  while (pos!=string::npos) {
    pos = content.find(key.c_str(), pos);
    if (pos!=string::npos) {
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
          if(line_tolower.find("queue")!=string::npos) {
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
 * \return raises an exception on error
 */
int JobServer::cancelJob()
{
  msessionServer.check(); //To check the sessionKey
  std::string acLogin;
  std::string machineName;
  std::string jobSerialized;
  BatchType   batchType;
  std::string initialJobId;
  std::string jobId;
  std::string owner;
  int status;
  std::vector<std::string> results;
  std::vector<std::string>::iterator  iter;
  acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);
  machineName =  getMachineName(mmachineId);

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  std::string sqlCancelRequest;
  initialJobId = mjob.getJobId();
  if(initialJobId.compare("all") != 0 &&
     initialJobId.compare("ALL") != 0) {

    sqlCancelRequest = "SELECT owner, status, jobId, batchJobId, vmId, batchType"
                       " FROM job, vsession"
                       " WHERE vsession.numsessionid=job.vsession_numsessionid"
                       " AND jobId='"+mjob.getJobId()+"'";
  } else {
    if(userServer.isAdmin()) {
      sqlCancelRequest = "SELECT owner, status, jobId, batchJobId, vmId, batchType"
                         " FROM job, vsession "
                         " WHERE vsession.numsessionid=job.vsession_numsessionid"
                         " AND status < 5"
                         " AND submitMachineId='"+mmachineId+"'" ;
    } else {
      sqlCancelRequest = "SELECT owner, status, jobId, batchJobId, vmId, batchType"
                         " FROM job, vsession"
                         " WHERE vsession.numsessionid=job.vsession_numsessionid"
                         " AND status < 5"
                         " AND owner='"+acLogin+"'"
                         " AND submitMachineId='"+mmachineId+"'" ;
    }
  }


  boost::scoped_ptr<DatabaseResult> sqlCancelResult(mdatabaseVishnu->getResult(sqlCancelRequest.c_str()));
  if (sqlCancelResult->getNbTuples() != 0) {
    for (size_t i = 0; i < sqlCancelResult->getNbTuples(); ++i) {
      results.clear();
      results = sqlCancelResult->get(i);

      iter = results.begin();
      owner = *iter;
      if (userServer.isAdmin()) {
        acLogin = owner;
      } else if(owner.compare(acLogin)!=0) {
        throw TMSVishnuException(ERRCODE_PERMISSION_DENIED);
      }

      ++iter;
      status = convertToInt(*iter);
      if(status == vishnu::STATE_COMPLETED) {
        throw TMSVishnuException(ERRCODE_ALREADY_TERMINATED, mjob.getJobId());
      }

      if(status == vishnu::STATE_CANCELLED) {
        throw TMSVishnuException(ERRCODE_ALREADY_CANCELED, mjob.getJobId());
      }

      ++iter;
      jobId = *iter;
      ++iter;
      mjob.setJobId(*iter); //To reset the jobId
      ++iter;
      mjob.setVmId(*iter);
      ++iter;
      batchType = static_cast<BatchType>(convertToInt(*iter));
      ::ecorecpp::serializer::serializer jobSer;
      jobSerialized =  jobSer.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

      SSHJobExec sshJobExec(acLogin, machineName,
                            batchType,
                            mbatchVersion,  // it will work for POSIX at the POSIX backend ignores the batch version
                            jobSerialized);
      sshJobExec.sshexec("CANCEL");

      std::string errorInfo = sshJobExec.getErrorInfo();

      if(errorInfo.size() !=0 &&
         (initialJobId.compare("all")!=0 &&
          initialJobId.compare("ALL")!=0)) {
        int code;
        std::string message;
        scanErrorMessage(errorInfo, code, message);
        throw TMSVishnuException(code, message);
      } else if(errorInfo.size()==0) {
        std::string sqlUpdatedRequest = ""
                                        "UPDATE job SET status="+vishnu::convertToString(vishnu::STATE_CANCELLED)+""
                                        " WHERE jobId='"+jobId+"'";
        mdatabaseVishnu->process(sqlUpdatedRequest.c_str());
      }
    }
  } else {
    if(initialJobId.compare("all")!=0 && initialJobId.compare("ALL")!=0) {
      throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
    }
  }

  return 0;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job JobServer::getJobInfo() {

  //To check the sessionKey
  msessionServer.check();

  std::vector<std::string> results;
  std::vector<std::string>::iterator  iter;
  std::string sqlRequest =
      "SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, jobPath, workId, "
      "  outputPath, errorPath, outputDir, jobPrio, nbCpus, jobWorkingDir, job.status, "
      "  submitDate, endDate, owner, jobQueue,wallClockLimit, groupName, jobDescription, "
      "  memLimit, nbNodes, nbNodesAndCpuPerNode, batchJobId, userid, vmId, vmIp"
      " FROM job, vsession, users "
      " WHERE vsession.numsessionid=job.vsession_numsessionid "
      " AND vsession.users_numuserid=users.numuserid"
      " AND job.status >= 0"
      " AND job.submitMachineId='"+mmachineId+"'"
      " AND job.jobId='"+mjob.getJobId()+"'";

  boost::scoped_ptr<DatabaseResult> sqlResult(mdatabaseVishnu->getResult(sqlRequest.c_str()));

  if (sqlResult->getNbTuples() == 0) {
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
  }

  results.clear();
  results = sqlResult->get(0);
  iter = results.begin();

  mjob.setSessionId(*iter);
  mjob.setSubmitMachineId(*(++iter));
  mjob.setSubmitMachineName(*(++iter));
  mjob.setJobId(*(++iter));
  mjob.setJobName(*(++iter));
  mjob.setJobPath(*(++iter));
  mjob.setWorkId(convertToLong(*(++iter)));
  mjob.setOutputPath(*(++iter));
  mjob.setErrorPath(*(++iter));
  mjob.setOutputDir(*(++iter));
  mjob.setJobPrio(convertToInt(*(++iter)));
  mjob.setNbCpus(convertToInt(*(++iter)));
  mjob.setJobWorkingDir(*(++iter));
  mjob.setStatus(convertToInt(*(++iter)));
  mjob.setSubmitDate(convertLocaltimeINUTCtime(convertToTimeType(*(++iter)))); //convert the submitDate into UTC date
  mjob.setEndDate(convertLocaltimeINUTCtime(convertToTimeType(*(++iter)))); //convert the endDate into UTC date
  mjob.setOwner(*(++iter));
  mjob.setJobQueue(*(++iter));
  mjob.setWallClockLimit(convertToInt(*(++iter)));
  mjob.setGroupName(*(++iter));
  mjob.setJobDescription(*(++iter));
  mjob.setMemLimit(convertToInt(*(++iter)));
  mjob.setNbNodes(convertToInt(*(++iter)));
  mjob.setNbNodesAndCpuPerNode(*(++iter));
  mjob.setBatchJobId(*(++iter));
  mjob.setUserId(*(++iter));
  mjob.setVmId(*(++iter));
  mjob.setVmIp(*(++iter));

  return mjob;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job JobServer::getData()
{
  return mjob;
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
    if (codeInString.size()!=0) {
      try {
        code = boost::lexical_cast<int>(codeInString);
      }
      catch(boost::bad_lexical_cast &){
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

  if (date.size()==0 ||
      // For mysql, the empty date is 0000-00-00, not empty, need this test to avoid problem in ptime
      date.find("0000-00-00")!=std::string::npos) {
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


/**
 * \brief Function to set the path of output directory
 * \param parentDir The directory in which to create the output dir
 * \param dirSuffix the suffix of the output dir
 * \param content the script content to be update which the generated path
 */
void JobServer::computeOutputDir(const std::string& parentDir,
                             const std::string& dirSuffix,
                             std::string& content) {

  std::string prefix = (boost::algorithm::ends_with(parentDir, "/"))? "OUTPUT_" : "/OUTPUT_" ;
  std::string outdir = parentDir + prefix + dirSuffix ;
  vishnu::replaceAllOccurences(content, "$VISHNU_OUTPUT_DIR", outdir);
  vishnu::replaceAllOccurences(content, "${VISHNU_OUTPUT_DIR}", outdir);
  mjob.setOutputDir(outdir) ;
  setenv("VISHNU_OUTPUT_DIR", outdir.c_str(), 1);
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
void JobServer::treatSpecificParams(const std::string& specificParams,
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
 * \brief Function to save the encapsulated job into the database
 */
void JobServer::recordJob2db()
{
  if (mjob.getSessionId().empty()) {
    throw TMSVishnuException(ERRCODE_AUTHENTERR, "Cannot record to database. Empty session id. Previous error: "+mjob.getErrorPath());
  }
  std::string sqlUpdate = "UPDATE job set ";
  sqlUpdate+="vsession_numsessionid='"+mjob.getSessionId()+"', ";
  sqlUpdate+="submitMachineId='"+mmachineId+"', ";
  sqlUpdate+="submitMachineName='"+mjob.getSubmitMachineName()+"', ";
  sqlUpdate+="batchJobId='"+mjob.getBatchJobId()+"', ";
  sqlUpdate+="batchType="+convertToString(mbatchType)+", ";
  sqlUpdate+="jobName='"+mjob.getJobName()+"', ";
  sqlUpdate+="jobPath='"+mjob.getJobPath()+"', ";
  sqlUpdate+="outputPath='"+mjob.getOutputPath()+"',";
  sqlUpdate+="errorPath='"+mjob.getErrorPath()+"',";
  sqlUpdate+="scriptContent='job', ";
  sqlUpdate+="jobPrio="+convertToString(mjob.getJobPrio())+", ";
  sqlUpdate+="nbCpus="+convertToString(mjob.getNbCpus())+", ";
  sqlUpdate+="jobWorkingDir='"+mjob.getJobWorkingDir()+"', ";
  sqlUpdate+="status="+convertToString(mjob.getStatus())+", ";
  sqlUpdate+="submitDate=CURRENT_TIMESTAMP, ";
  sqlUpdate+="owner='"+mjob.getOwner()+"', ";
  sqlUpdate+="jobQueue='"+mjob.getJobQueue()+"', ";
  sqlUpdate+="wallClockLimit="+convertToString(mjob.getWallClockLimit())+", ";
  sqlUpdate+="groupName='"+mjob.getGroupName()+"',";
  sqlUpdate+="jobDescription='"+mjob.getJobDescription()+"', ";
  sqlUpdate+="memLimit="+convertToString(mjob.getMemLimit())+", ";
  sqlUpdate+="nbNodes="+convertToString(mjob.getNbNodes())+", ";
  sqlUpdate+="nbNodesAndCpuPerNode='"+mjob.getNbNodesAndCpuPerNode()+"', ";
  sqlUpdate+="outputDir='"+mjob.getOutputDir()+"', ";
  sqlUpdate+= mjob.getWorkId()? "workId="+convertToString(mjob.getWorkId())+", " : "";
  sqlUpdate+="vmId='"+mjob.getVmId()+"', ";
  sqlUpdate+="vmIp='"+mjob.getVmIp()+"' ";
  sqlUpdate+="WHERE jobid='"+mjob.getJobId()+"';";
  mdatabaseVishnu->process(sqlUpdate);
}

/**
 * \brief Function to get the hostname of a machine id
 * \param machineId Id of the machine
 * \return The machine name or throw exception on error
 */
std::string JobServer::getMachineName(const std::string& machineId){
  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  MachineServer machineServer(machine);
  std::string machineName = machineServer.getMachineName();
  delete machine;
  return machineName;
}

/**
 * \brief Function to set the Working Directory
 * \param options the options to submit job
 * \param suffix the suffix of the working directory
 * \return the script path
 */
std::string JobServer::computeWorkingDir(TMS_Data::SubmitOptions& options, const std::string& suffix)
{
  std::string workingDir = boost::filesystem::temp_directory_path().string();
    string scriptPath = "";
  if(mbatchType == DELTACLOUD) {
    string mountPoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], false);
    workingDir = mountPoint + "/" + suffix;
    string inputDir =  workingDir + "/INPUT";
    scriptPath = inputDir + "/script.xsh";
    vishnu::createDir(workingDir, true); // create the working directory
    vishnu::createDir(inputDir, true); // create the input directory
    string directory = "";
    try {
      directory = vishnu::moveFileData(options.getFileParams(), inputDir);
    } catch(bfs::filesystem_error &ex) {
      throw SystemException(ERRCODE_RUNTIME_ERROR, ex.what());
    }
    if(directory.length() > 0) {
      std::string fileparams = options.getFileParams();
      vishnu::replaceAllOccurences(fileparams, directory, inputDir);
      options.setFileParams(fileparams);
    }
  } else {
    scriptPath = workingDir +"/"+ bfs::unique_path("job_script%%%%%%").string();
    std::string home = UserServer(msessionServer).getUserAccountProperty(mmachineId, "home");
    workingDir = (options.getWorkingDir().empty()? home : options.getWorkingDir()) ;
  }
  options.setWorkingDir(workingDir);
  return scriptPath;
}

/**
 * \brief Function to deserialize job
 * \param jobSerialized the Serialized job
 */

void JobServer::deserializeJob(std::string jobSerialized)
{
   TMS_Data::Job_ptr job = NULL;
  if (!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
    throw SystemException(ERRCODE_INVDATA, "JobServer::submitJob : job object is not well built");
  }
  mjob = *job;
  delete job;
}

/**
 * \brief Function to process the script with options
 * \param the script content
 * \param options the options to submit job
 * \param defaultBatchOption The default batch options
 * \return the processed script content
 */
std::string JobServer::processScript(std::string& scriptContent,
                                     TMS_Data::SubmitOptions& options,
                                     const std::vector<std::string>& defaultBatchOption,
                                     const std::string& machineName)
{
  std::string convertedScript;

  vishnu::replaceAllOccurences(scriptContent, "$VISHNU_SUBMIT_MACHINE_NAME", machineName);
  vishnu::replaceAllOccurences(scriptContent, "${VISHNU_SUBMIT_MACHINE_NAME}", machineName);

  if(options.getTextParams().size()) {
    vishnu::setParams(scriptContent, options.getTextParams()) ;
  }
  if(options.getFileParams().size()) {
    vishnu::setParams(scriptContent, options.getFileParams()) ;
  }
  boost::shared_ptr<ScriptGenConvertor> scriptConvertor(vishnuScriptGenConvertor(mbatchType, scriptContent));
  if(scriptConvertor->scriptIsGeneric()) {
    std::string genScript = scriptConvertor->getConvertedScript();
    convertedScript = genScript;
  } else {
    convertedScript = scriptContent;
  }
  std::string sep = " ";
  std::string directive = getBatchDirective(sep);
  if (!options.getSpecificParams().empty()) {
    treatSpecificParams(options.getSpecificParams(), convertedScript);
  }
  if (!defaultBatchOption.empty()){
    processDefaultOptions(defaultBatchOption, convertedScript, directive);
  }
  if(mbatchType == DELTACLOUD) {
    vishnu::replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", mjob.getOutputDir()+"/NODEFILE");
    vishnu::replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", mjob.getOutputDir()+"/NODEFILE");
  }

  return convertedScript;
}
