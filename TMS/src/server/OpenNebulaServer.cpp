/*
 * OpenNebulaServer.cpp
 *
 *  Created on: 27 nov. 2012
 *      Author: r
 */

#include <string.h>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "constants.hpp"
#include "utilServer.hpp"
#include "tmsUtils.hpp"
#include "SSHJobExec.hpp"
#include "OpenNebulaServer.hpp"
#include "TMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#include "OneRPCManager.hpp"
#include "OneCloudInstance.hpp"
#include "Logger.hpp"


OpenNebulaServer::OpenNebulaServer()
  : mcloudUser(""),
    mcloudUserPassword(""),
    mvmImageId(""),
    mvmFlavor(""),
    mvmUser(""),
    mvmUserKey(""),
    mnfsServer(""),
    mnfsMountPoint("")
{
  mcloudEndpoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
  mcloudUser= vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
  mcloudUserPassword = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);
  mcontextInitScript = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_CONTEXT_INIT], false);
  mvirtualNetwork = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VIRTUAL_NET], false);
  mvirtualNetworkMask = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VIRTUAL_NET_MASK], true);
  mvirtualNetworkGateway = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VIRTUAL_NET_GATEWAY], true);
  mvirtualNetworkDns = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VIRTUAL_NET_DNS], true);
}

OpenNebulaServer::~OpenNebulaServer() {
}

/**
 * \brief Function to submit a job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by submission function
 * \return raises an exception on error
 */
int
OpenNebulaServer::submit(const std::string& scriptPath,
                         const TMS_Data::SubmitOptions& options,
                         TMS_Data::ListJobs& jobSteps,
                         char** envp)
{
  mjobId = vishnu::getVar("VISHNU_JOB_ID", false);
  mjobOutputDir = vishnu::getVar("VISHNU_OUTPUT_DIR", false);
  handleCloudInfo(options);
  setupJobDataDir(mjobId, scriptPath);

  replaceEnvVariables(scriptPath);
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.allocate");
  rpcManager.addParam(getSessionString());
  rpcManager.addParam(generateKvmTemplate(options));
  rpcManager.addParam(false);   // to create VM on pending state
  rpcManager.execute();

  if (! rpcManager.lastCallSucceeded()) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, rpcManager.getStringResult());
  }

  TMS_Data::Job_ptr jobPtr = new TMS_Data::Job();

  OneCloudInstance oneCloud(mcloudEndpoint, getSessionString());
  VmT vmInfo;
  jobPtr->setVmId(vishnu::convertToString( rpcManager.getIntResult() ));
  if (oneCloud.loadVmInfo(vishnu::convertToInt(jobPtr->getVmId()), vmInfo) == 0) {
    jobPtr->setVmIp(vmInfo.ipAddr);
  }

  LOG(boost::str(boost::format("[INFO] Virtual machine created. ID: %1%, IP: %2%"
                               ) %  jobPtr->getVmId() % jobPtr->getVmIp()), LogInfo);

  //FIXME: job.setBatchJobId(vishnu::convertToString(jobPid));
  jobPtr->setStatus(vishnu::STATE_SUBMITTED);
  jobPtr->setJobName(returnInputOrDefaultIfEmpty(options.getName(), "PID_"+jobPtr->getBatchJobId()));
  jobPtr->setOutputPath(jobPtr->getOutputDir()+"/stdout");
  jobPtr->setErrorPath(jobPtr->getOutputDir()+"/stderr");
  jobPtr->setNbNodes(1);
  jobPtr->setOwner(mvmUser);

  jobSteps.getJobs().push_back(jobPtr);
  return 0;
}

/**
 * \brief Function to cancel job:  just shutdown and destroy the related VM
 * \param vmId the VM ID
 * \param jobDescr the description of the job in the form of jobId@vmId
 * \return raises an exception on error
 */
int
OpenNebulaServer::cancel(const std::string& vmId)
{
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.action");
  rpcManager.addParam(getSessionString());
  rpcManager.addParam(std::string("delete"));
  rpcManager.addParam(vishnu::convertToInt(vmId));
  rpcManager.execute();

  if (! rpcManager.lastCallSucceeded()) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, rpcManager.getStringResult());
  }

  LOG(boost::str(boost::format("[INFO] VM deleted: %1%") % vmId), LogInfo);
  return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobJsonSerialized the job structure encoded in json
 * \return -1 if the job is unknown or server not unavailable
 */
int
OpenNebulaServer::getJobState(const std::string& jobSerialized) {

  int jobStatus = vishnu::STATE_UNDEFINED;

  // Get input job infos
  JsonObject job(jobSerialized);
  std::string jobId = job.getStringProperty("jobid");
  std::string pid = job.getStringProperty("batchjobid");
  std::string owner = job.getStringProperty("owner");
  std::string vmId = job.getStringProperty("vmid");
  std::string vmIp = job.getStringProperty("vmip");

  if (! vmIp.empty()) {
    // retrive vm info
    VmT vmInfo;
    OneCloudInstance cloudInstance(mcloudEndpoint, getSessionString());

    if (cloudInstance.loadVmInfo(vishnu::convertToInt(vmId), vmInfo) == 0) {
      switch (vmInfo.state) {
        case VM_ACTIVE:
          jobStatus = monitorScriptState(jobId, pid, vmIp, owner);
          break;
        case VM_POWEROFF:
        case VM_FAILED:
        case VM_STOPPED:
        case VM_DONE:
          jobStatus = vishnu::STATE_FAILED;
          break;
        case VM_INIT:
        case VM_HOLD:
        case VM_UNDEPLOYED:
          jobStatus = vishnu::STATE_SUBMITTED;
        default:
          break;
      }
    }
    if (jobStatus == vishnu::STATE_CANCELLED
        || jobStatus == vishnu::STATE_COMPLETED
        || jobStatus == vishnu::STATE_FAILED) {

      LOG(boost::str(boost::format("[WARN] Cleaning job %1%; VM ID: %2%; VM State: %3%; Script Status: %4%.")
                     % jobId
                     % vmId
                     % vmState2String(vmInfo.state)
                     % vishnu::statusToString(jobStatus)), LogWarning);

      releaseResources(vmId);
    }
  } else {
    LOG(boost::str(boost::format("[WARN] Unable to monitor job: %1%, VMID: %2%."
                                 " Empty vm address") % jobId % vmId), LogWarning);
    jobStatus = vishnu::STATE_UNDEFINED;
  }
  return jobStatus;
}

/**
 * \brief Function to get the start time of the job
 * \param jobJsonSerialized The job structure encoded in json
 * \return 0 if the job is unknown
 */
time_t
OpenNebulaServer::getJobStartTime(const std::string& jobJsonSerialized) {

  //FIXME:
  long long startTime = 0;
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.info");
  rpcManager.addParam(getSessionString());

  JsonObject jobJson(jobJsonSerialized);
  rpcManager.addParam( jobJson.getStringProperty("vmid") );

  rpcManager.execute();
  return startTime;
}

/**
 * \brief This function get the information about different cloud endpoint
 * \param queueName (optional) Queue name
 * \return The list of cloud information
 */
TMS_Data::ListQueues_ptr
OpenNebulaServer::listQueues(const std::string& queueName) {

  OneCloudInstance cloud(mcloudEndpoint, getSessionString());

  TMS_Data::ListQueues_ptr queues = new TMS_Data::ListQueues();
  TMS_Data::Queue* queue = cloud.getQueueInfo();
  if (! queue)  {
    TMSVishnuException(ERRCODE_RUNTIME_ERROR, "Null queue info");
  }
  queue->setName(queueName);
  queues->getQueues().push_back(queue);
  return queues;
}


/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore
 */
void OpenNebulaServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                                      const std::vector<std::string>& ignoredIds) { }


int
create_plugin_instance(void **instance)
{
  try {
    *instance = new OpenNebulaServer;
  } catch (const std::bad_alloc& e) {
    return 1;
  }
  return PLUGIN_OK;
}


/**
 * \brief Function for cleaning up virtual machine
 * \param vmId The id of the virtual machine
 */
void OpenNebulaServer::releaseResources(const std::string& vmId)
{
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.action");
  rpcManager.addParam(getSessionString());
  rpcManager.addParam(std::string("delete"));
  rpcManager.addParam(vishnu::convertToInt(vmId));
  rpcManager.execute();
  if (! rpcManager.lastCallSucceeded()) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, rpcManager.getStringResult());
  }
  //FIXME: check that the vm is shutdown and clear it
  LOG(boost::str(boost::format("[INFO] VM deleted: %1%") % vmId), LogInfo);
}

/**
 * \brief To retrieve specific submission parameters
 * \param specificParamss The string containing the list of parameters
 */
void OpenNebulaServer::retrieveUserSpecificParams(const std::string& specificParams) {
  ListStrings listParams;
  boost::split(listParams, specificParams, boost::is_any_of(" "));
  for (ListStrings::iterator it = listParams.begin(); it != listParams.end(); ++it) {
    size_t pos = it->find("=");
    if (pos != std::string::npos) {
      std::string param = it->substr(0, pos);
      std::string value = it->substr(pos+1, std::string::npos);
      if (param == "endpoint") {
        mcloudEndpoint = value;
      } else if (param == "user") {
        mcloudUser = value;
      } else if (param == "user-password") {
        mcloudUserPassword = value;
      } else if (param == "vm-image") {
        mvmImageId = value;
      } else if (param == "vm-user") {
        mvmUser = value;
      } else if (param == "vm-key") {
        mvmUserKey = value;
      } else if (param == "vm-flavor") {
        mvmFlavor = value;
      } else if (param == "nfs-server") {
        mnfsServer = value;
      } else if (param == "nfs-mountpoint") {
        mnfsMountPoint = value;
      } else {
        throw TMSVishnuException(ERRCODE_INVALID_PARAM, param);
      }
    }
  }
}

/**
 * \brief Function to replace some environment varia*bles in a string
 * \param scriptContent The string content to modify
 */
void OpenNebulaServer::replaceEnvVariables(const std::string& scriptPath) {
  std::string scriptContent = vishnu::get_file_content(scriptPath);

  //To replace VISHNU_BATCHJOB_ID
  vishnu::replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$$");
  vishnu::replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$$");
  //To replace VISHNU_BATCHJOB_NAME
  vishnu::replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$(ps -o comm= -C -p $$)");
  vishnu::replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$(ps -o comm= -C -p $$)");
  //To replace VISHNU_BATCHJOB_NUM_NODES. Depends on the number of nodes in VISHNU_BATCHJOB_NODEFILE
  //Note: The variable VISHNU_BATCHJOB_NODEFILE is set later in JobServer
  vishnu::replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$(wc -l ${VISHNU_BATCHJOB_NODEFILE} | cut -d' ' -f1)");
  vishnu::replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$(wc -l ${VISHNU_BATCHJOB_NODEFILE} | cut -d' ' -f1)");

  vishnu::saveInFile(scriptPath, scriptContent);
}

/**
 * @brief Return a string as expected by OpenNebula API (username:password)
 * @return string
 */
std::string
OpenNebulaServer::getSessionString(void)
{
  if (mcloudUser.empty() && mcloudUserPassword.empty()) {
    throw TMSVishnuException(ERRCODE_INVALID_PARAM,
                             "Either the username or the password to authenticate against OpenNebula is empty");
  }
  return boost::str(boost::format("%1%:%2%") % mcloudUser % mcloudUserPassword);
}

/**
 * @brief return a VM template for KVM virtual machine
 * @param options submit options
 * @return
 */
std::string
OpenNebulaServer::generateKvmTemplate(const TMS_Data::SubmitOptions& options)
{
  std::string pubkey = vishnu::get_file_content(vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], false));
  return boost::str(
        boost::format(
          "NAME=\"vishnu-vm\"                                                    \n"
          "CPU=%1%                                                               \n"
          "VCPU=%1%                                                              \n"
          "MEMORY=%2%                                                            \n"
          "DISK = [ IMAGE = \"%3%\", DRIVER=\"qcow2\"]                           \n"
          "OS=[                                                                  \n"
          "  ARCH=\"i686\",                                                      \n"
          "  ROOT=\"sda1\",                                                      \n"
          "  BOOT=\"hd,fd,cdrom,network\" ]                                      \n"
          "NIC = [NETWORK=\"%4%\"]                                               \n"
          "GRAPHICS = [TYPE=\"vnc\", LISTEN=\"0.0.0.0\",KEYMAP=\"fr\"]           \n"
          "RAW=[                                                                 \n"
          "  TYPE=\"kvm\",                                                       \n"
          "  DATA=\"                                                             \n"
          "    <serial type='pty'><target port='0'/></serial>                    \n"
          "    <console type='pty'><target type='serial' port='0'/></console>\"] \n"
          "CONTEXT=[                                                             \n"
          "  HOSTNAME=\"vm-$VMID\",                                              \n"
          "  NETWORK=\"YES\",                                                    \n"
          "  ETH0_IP=\"$NIC[IP, NETWORK=\\\"%4%\\\"]\",                          \n"
          "  ETH0_NETMASK=\"%5%\",                                               \n"
          "  ETH0_GATEWAY=\"%6%\",                                               \n"
          "  ETH0_DNS=\"%7%\",                                                   \n"
          "  FILES=\"%8%\",                                                      \n"
          "  USERNAME=\"%9%\",                                                   \n"
          "  SSH_PUBLIC_KEY=\"%10%\",                                            \n"
          "  USER_PUBKEY=\"%11%\",                                               \n"
          "  DATA_SERVER=\"%12%\",                                               \n"
          "  DATA_MOUNT_POINT=\"%13%\",                                          \n"
          "  TARGET=\"hdb\"                                                      \n"
          "]")
        % returnInputOrDefaultIfNegativeNull(options.getNbCpu(), 1)
        % returnInputOrDefaultIfNegativeNull(options.getMemory(), 512)
        % mvmImageId
        % mvirtualNetwork
        % mvirtualNetworkMask
        % mvirtualNetworkGateway
        % mvirtualNetworkDns
        % mcontextInitScript
        % mvmUser
        % pubkey
        % pubkey
        % mnfsServer
        % mbaseDataDir);
}


/**
 * @brief Return the defaultValue if a given string is empty
 * @param value The input string
 * @param defaultValue The default value
 * @return string
 */
std::string
OpenNebulaServer::returnInputOrDefaultIfEmpty(const std::string& value, const std::string& defaultValue)
{
  if (value.empty()) {
    return defaultValue;
  }
  return value;
}

/**
 * @brief Return the defaultValue if a given string is empty
 * @param value The input string
 * @param defaultValue The default value
 * @return int
 */
int
OpenNebulaServer::returnInputOrDefaultIfNegativeNull(int value, int defaultValue)
{
  if (value <= 0) {
    return defaultValue;
  }
  return value;
}


/**
 * @brief OpenNebulaServer::monitorScriptState
 * @param jobId
 * @param pid
 * @param vmIp
 * @param uid
 * @return
 */
int
OpenNebulaServer::monitorScriptState(const std::string& jobId,
                                     const std::string& pid,
                                     const std::string& vmIp,
                                     const std::string& uid)
{
  int jobStatus = vishnu::STATE_UNDEFINED;
  SSHJobExec sshEngine(uid, vmIp);
  if (sshEngine.isReadyConnection()) {
    if (! pid.empty()) {
      std::string statusFile = boost::str(boost::format("/tmp/%1%-%2%@%3%") % jobId % pid % vmIp);
      std::string cmd = boost::str(boost::format("ps -o pid= -p %1% | wc -l > %2%") % pid % statusFile);

      sshEngine.execCmd(cmd, false);

      if (vishnu::getStatusValue(statusFile) == 0) {
        jobStatus = vishnu::STATE_COMPLETED;
      } else {
        jobStatus = vishnu::STATE_RUNNING;
      }
      vishnu::deleteFile(statusFile.c_str());
    } else {
      //FIXME: think mechanism to retrieve the PID of the process.
      // Should be think with the contextualization
      LOG(boost::str(boost::format("[WARN] Empty PID, Job ID: %1%") %jobId), LogWarning);
    }
  }
  return jobStatus;
}

/**
 * @brief Gives the equivalent string of a VM state
 * @param state The state
 * @return a string
 */
std::string
OpenNebulaServer::vmState2String(int state)
{
  std::string value = "UNDEFINED";
  switch (state) {
    case VM_ACTIVE:
      value= "RUNNING";
      break;
    case VM_POWEROFF:
      value= "POWEROFF";
      break;
    case VM_FAILED:
      value= "FAILED";
      break;
    case VM_STOPPED:
      value= "STOPPED";
      break;
    case VM_DONE:
      value= "DONE";
      break;
    case VM_INIT:
      value= "INIT";
      break;
    case VM_HOLD:
      value= "HOLD";
      break;
    case VM_UNDEPLOYED:
      value= "UNDEPLOYED";
      break;
    default:
      value= "UNDEFINED";
      break;
  }

  return value;
}

/**
 * @brief handleCloudInfo
 * @param options
 */
void
OpenNebulaServer::handleCloudInfo(const TMS_Data::SubmitOptions& options)
{
  retrieveUserSpecificParams(options.getSpecificParams());

  // Get configuration parameters
  if (mvmImageId.empty()) {
    mvmImageId = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE], false);
  }
  if (mvmFlavor.empty()) {
    mvmFlavor = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR], false);
  }
  if (mvmUser.empty()) {
    mvmUser = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], false);
  }
  if (mvmUserKey.empty()) {
    mvmUserKey = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], false);
  }
  if (mnfsServer.empty()) {
    mnfsServer = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_SERVER], true);
  }
  if(mnfsMountPoint.empty()) {
    mnfsMountPoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], true);
  }

}
/**
 * @brief create job data directory containing script and misc files
 * @param jobId The job id.
 * @param scriptPath The script path
 * @return: Nothing. The base datadir is stored in the mbaseDataDir variable
 */
void
OpenNebulaServer::setupJobDataDir(const std::string& jobId, const std::string& scriptPath)
{
  mbaseDataDir = boost::str(boost::format("%1%/%2%") % mnfsMountPoint % jobId);
  std::string targetScriptPath = boost::str(boost::format("%1%/script.sh") % mbaseDataDir);
  vishnu::createDir(mbaseDataDir);
  vishnu::saveInFile(targetScriptPath, vishnu::get_file_content(scriptPath));
  vishnu::makeFileExecutable(targetScriptPath);
}


