/*
 * DeltaCloudServer.cpp
 *
 *  Created on: 27 nov. 2012
 *      Author: r
 */

#include <boost/format.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "DeltaCloudServer.hpp"
#include "TMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#include "SSHJobExec.hpp"
#include "string.h"
#include "common.h"
#include <vector>
#include "constants.hpp"
#include "utilServer.hpp"
#include "tmsUtils.hpp"


DeltaCloudServer::DeltaCloudServer()
  : mcloudUser(""),
    mcloudUserPassword(""),
    mvmImageId(""),
    mvmFlavor(""),
    mvmUser(""),
    mvmUserKey(""),
    mnfsServer(""),
    mnfsMountPoint("")
{
}

DeltaCloudServer::~DeltaCloudServer() {
  deltacloud_free(mcloudApi);
}

/**
 * \brief Function to submit a job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param jobSteps The result job steps
 * \param envp The list of environment variables used by submission function
 * \return raises an exception on error
 */
int
DeltaCloudServer::submit(const std::string& scriptPath,
                         const TMS_Data::SubmitOptions& options,
                         TMS_Data::ListJobs& jobSteps,
                         char** envp) {

  initialize(); // Initialize Delatacloud API

  mjobId = vishnu::getVar("VISHNU_JOB_ID", false);
  mjobOutputDir = vishnu::getVar("VISHNU_OUTPUT_DIR", false);

  retrieveSpecificParams(options.getSpecificParams()); // First set specific parameters
  replaceEnvVariables(scriptPath.c_str());

  // Get configuration parameters
  //FIXME: possibly memory leak if vishnu::getVar through exception. We may need to catch that and free deltacloud API by calling finalize()
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
  // Set the parameters of the virtual machine instance
  std::vector<deltacloud_create_parameter> params;
  deltacloud_create_parameter param;
  param.name = strdup("hwp_id");
  param.value = strdup(mvmFlavor.c_str());
  params.push_back(param);

  param.name = strdup("keyname");
  param.value = strdup(mvmUserKey.c_str());
  params.push_back(param);

  param.name = strdup("name");
  param.value = strdup(boost::str(boost::format("vishnu-job.vm.%1%") % mjobId).c_str());
  params.push_back(param);

  char *instid = NULL;
  if (deltacloud_create_instance(mcloudApi, mvmImageId.c_str(), &params[0], params.size(), &instid) < 0) {
    cleanUpParams(params); // cleanup allocated parameters
    finalize();
    std::string msg = (boost::format("Unable to create instance: %1%")%deltacloud_get_last_error_string()).str();
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }
  cleanUpParams(params);  // cleanup allocated parameters

  deltacloud_instance instance;
  if(wait_for_instance_boot(mcloudApi, instid, &instance) != 0) {
    std::string msg = (boost::format("Instance never went RUNNING; VM state: %1%\n")%instance.state).str();
    deltacloud_instance_destroy(mcloudApi, &instance);
    finalize();
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
  }

  deltacloud_address* instanceAddr = NULL;
  instanceAddr = instance.private_addresses ? instance.private_addresses : instance.public_addresses;

  if (! instanceAddr) {
    deltacloud_free_instance(&instance);
    finalize();
    std::string msg = (boost::format("Instance does not have network address %1%\n")%instance.id).str();
    throw TMSVishnuException(ERRCODE_UNKNOWN_BATCH_SCHEDULER, msg);
  }

  std::string nodeFile = boost::str(boost::format("%1%/NODEFILE") % mjobOutputDir);
  vishnu::saveInFile(nodeFile, instanceAddr->address); // Create the NODEFILE

  std::cout << boost::format("[TMS][INFO] Virtual machine started\n"
                             " ID: %1%\n"
                             " NAME: %2%\n"
                             " IP: %3%\n"
                             " Startime: %4%\n")%instance.id %instance.name %instanceAddr->address %instance.launch_time;

  // Create an ssh engine for the virtual machine & submit the script
  SSHJobExec sshEngine(mvmUser, instanceAddr->address);
  int jobPid = -1;
  try {
    jobPid = sshEngine.execRemoteScript(scriptPath.c_str(), mnfsServer, mnfsMountPoint, mjobOutputDir);
  } catch(...) {
    throw;
  }
  TMS_Data::Job_ptr jobPtr = new TMS_Data::Job();

  jobPtr->setBatchJobId(vishnu::convertToString(jobPid));
  jobPtr->setJobName("PID_"+jobPid);
  jobPtr->setBatchJobId(vishnu::convertToString(jobPid));
  jobPtr->setVmId(instance.id);
  jobPtr->setStatus(vishnu::STATE_SUBMITTED);
  jobPtr->setVmIp(instanceAddr->address);
  jobPtr->setOutputPath(boost::str(boost::format("%1%/stdout") % mjobOutputDir));
  jobPtr->setErrorPath(boost::str(boost::format("%1%/stderr") % mjobOutputDir));
  jobPtr->setNbNodes(1);

  jobSteps.getJobs().push_back(jobPtr);

  deltacloud_free_instance(&instance);
  finalize();

  return 0;
}

/**
 * \brief Function to cancel job:  just shutdown and destroy the related VM
 * \param vmId the VM ID
 * \return raises an exception on error
 */
int
DeltaCloudServer::cancel(const std::string& vmId) {
  try {
    releaseResources(vmId); // Stop the virtual machine to release resources
  } catch(...) {
    throw;
  }
  return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobJsonSerialized the job structure encoded in json
 * \return -1 if the job is unknown or server not unavailable
 */
int
DeltaCloudServer::getJobState(const std::string& jobSerialized) {

  JsonObject job(jobSerialized);
  std::string jobId = job.getStringProperty("jobid");
  std::string pid = job.getStringProperty("batchjobid");
  std::string vmUser = job.getStringProperty("owner");
  std::string vmId = job.getStringProperty("vmid");
  std::string vmIp = job.getStringProperty("vmip");

  SSHJobExec sshEngine(vmUser, vmIp);
  std::string statusFile = boost::str(boost::format("/tmp/%1%-%2%@%3%") % jobId % pid % vmIp);
  std::string cmd = (boost::format("ps -o pid= -p %1% | wc -l > %2%")%pid %statusFile).str();
  sshEngine.execCmd(cmd, false);

  // Check if the job is completed
  // If yes stop the virtual machine and release the resources
  int status = vishnu::STATE_RUNNING;
  if (vishnu::getStatusValue(statusFile) == 0) {
    releaseResources(vmId);
    status = vishnu::STATE_COMPLETED;
  }

  vishnu::deleteFile(statusFile.c_str());

  return status;
}

/**
 * \brief Function to get the start time of the job
 * \param jobJsonSerialized The job structure encoded in json
 * \return 0 if the job is unknown
 */
time_t
DeltaCloudServer::getJobStartTime(const std::string& jobJsonSerialized) {

  initialize(); // Initialize the Cloud API

  JsonObject jobJson(jobJsonSerialized);
  std::string vmid = jobJson.getStringProperty("vmid");

  deltacloud_instance instance; // Get the instance
  if (deltacloud_get_instance_by_id(mcloudApi, vmid.c_str(), &instance) < 0) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
  }

  long long startTime = 0;
  try {
    startTime = vishnu::convertStringToWallTime(instance.launch_time);
  } catch (...) {} // leave empty
  deltacloud_free_instance(&instance);
  finalize();
  return startTime;
}


/**
 * \brief Function to request the status of queues
 * \param optQueueName (optional) the name of the queue to request
 * \return The requested status in to ListQueues data structure
 */
TMS_Data::ListQueues*
DeltaCloudServer::listQueues(const std::string& optQueueName) {

  //TODO The semantic is no yet defined
  return new TMS_Data::ListQueues();
}


/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore
 */
void DeltaCloudServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                                      const std::vector<std::string>& ignoredIds) { }


int
create_plugin_instance(void **instance) {
  try {
    *instance = new DeltaCloudServer;
  } catch (const std::bad_alloc& e) {
    return 1;
  }
  return PLUGIN_OK;
}


/**
 * \brief Function for initializing the deltacloud API
 */
void DeltaCloudServer::initialize(void) {

  if(mcloudEndpoint.empty()) {
    mcloudEndpoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
  }
  if(mcloudUser.empty()) {
    mcloudUser= vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
  }
  if(mcloudUserPassword.empty()) {
    mcloudUserPassword = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);
  }
  mcloudApi =  new deltacloud_api;
  if (deltacloud_initialize(mcloudApi,
                            const_cast<char*>(mcloudEndpoint.c_str()),
                            const_cast<char*>(mcloudUser.c_str()),
                            const_cast<char*>(mcloudUserPassword.c_str())) < 0) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
  }
}


/**
 * \brief Function for cleaning up virtual machine
 * \param vmid The id of the virtual machine
 */
void DeltaCloudServer::releaseResources(const std::string & vmid) {

  initialize(); // Initialize delta cloud
  deltacloud_instance instance; // Get the instance
  if (deltacloud_get_instance_by_id(mcloudApi, vmid.c_str(), &instance) < 0) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             (boost::format("Get instance failed with the following reason (%1%)")%deltacloud_get_last_error_string()).str());
  }
  std::cout << boost::format("[TMS][INFO] The instance %1% (NAME: %2%) will be stopped")%instance.id%instance.name;
  if (deltacloud_instance_destroy(mcloudApi, &instance) < 0) { // Stop the instance
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             (boost::format("Deleting the virtual machine failed (%1%)")%deltacloud_get_last_error_string()).str());
  }
  deltacloud_free_instance(&instance);
  finalize();
}


/**
 * \brief Function for cleaning up the allocated dynamic data structure
 */
void DeltaCloudServer::finalize() {
  deltacloud_free(mcloudApi);
  delete mcloudApi;
}

/**
 * \brief Function for cleaning up a deltacloud params list
 * \param: params The list of params
 */
void DeltaCloudServer::cleanUpParams(std::vector<deltacloud_create_parameter>& params) {
  for(int i=0; i< params.size(); i++) {
    free(params[i].name);
    free(params[i].value);
  }
}

/**
 * \brief To retrieve specific submission parameters
 * \param specificParamss The string containing the list of parameters
 */
void DeltaCloudServer::retrieveSpecificParams(const std::string& specificParams) {
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
void DeltaCloudServer::replaceEnvVariables(const char* scriptPath) {
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
