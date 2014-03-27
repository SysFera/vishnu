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
  if(mcloudEndpoint.empty()) {
    mcloudEndpoint = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
  }
  if(mcloudUser.empty()) {
    mcloudUser= vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
  }
  if(mcloudUserPassword.empty()) {
    mcloudUserPassword = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);
  }
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
  replaceEnvVariables(scriptPath.c_str());
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.allocate");
  rpcManager.addParam(getSessionString());
  rpcManager.addParam(getKvmTemplate(options));
  rpcManager.addParam(true);   // to create VM on hold state
  rpcManager.execute();

  TMS_Data::Job_ptr jobPtr = new TMS_Data::Job();
  jobPtr->setVmId(vishnu::convertToString(rpcManager.getIntResult()));
  //FIXME: job.setBatchJobId(vishnu::convertToString(jobPid));
  //FIXME: job.setBatchJobId(vishnu::convertToString(jobPid));
  jobPtr->setJobName("PID_"+jobPtr->getBatchJobId());
  jobPtr->setStatus(vishnu::STATE_SUBMITTED);
  //FIXME: job.setVmIp(instanceAddr->address);
  jobPtr->setOutputPath(jobPtr->getOutputDir()+"/stdout");
  jobPtr->setErrorPath(jobPtr->getOutputDir()+"/stderr");
  jobPtr->setNbNodes(1);

  std::cout << boost::format("[TMS][INFO] Virtual machine create"
                             " ID: %1%\n") % rpcManager.getIntResult();
  jobSteps.getJobs().push_back(jobPtr);
  return 0;
}

/**
 * \brief Function to cancel job
 * \param jobDescr the description of the job in the form of jobId@vmId
 * \return raises an exception on error
 */
int
OpenNebulaServer::cancel(const std::string& vmId)
{
  releaseResources(vmId);
  return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobDescr the job description in the form of pid@user@vmaddress@vmId
 * \return -1 if the job is unknown or server not unavailable
 */
int
OpenNebulaServer::getJobState(const std::string& jobDescr) {

  // Get job infos
  ListStrings jobInfos = getJobInfos(jobDescr, 4);
  std::string pid = jobInfos[0];
  std::string vmUser = jobInfos[1];
  std::string vmIp = jobInfos[2];
  std::string vmId = jobInfos[3];

  SSHJobExec sshEngine(vmUser, vmIp);
  std::string statusFile = "/tmp/"+jobDescr;
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
 * \param jobDescr the description of the job in the form of jobId@vmId
 * \return 0 if the job is unknown
 */
time_t
OpenNebulaServer::getJobStartTime(const std::string& jobDescr) {

  long long startTime = 0;
  ListStrings jobInfos = getJobInfos(jobDescr, 2); // Get the job information
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.info");
  rpcManager.addParam(getSessionString());
  rpcManager.addParam(vishnu::convertToInt(jobInfos[1]));
  rpcManager.execute();
  return startTime;
}

/**
 * \brief Function to request the status of queues
 * \param optQueueName (optional) the name of the queue to request
 * \return The requested status in to ListQueues data structure
 */
TMS_Data::ListQueues*
OpenNebulaServer::listQueues(const std::string& optQueueName) {

  //TODO The semantic is no yet defined
  return new TMS_Data::ListQueues();
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
 * \param vmid The id of the virtual machine
 */
void OpenNebulaServer::releaseResources(const std::string & vmid)
{
  OneRPCManager rpcManager(mcloudEndpoint);
  rpcManager.setMethod("one.vm.action");
  rpcManager.addParam("shutdown");
  rpcManager.addParam(vishnu::convertToInt(vmid));
  rpcManager.execute();
  //FIXME: check that the vm is shutdown and clear it
}

/**
 * \brief Function to decompose job information
 * \param: jobDescr The description of the job in the form of param1@param2@...
 * \param: numParams The number of expected parameters
 */
ListStrings OpenNebulaServer::getJobInfos(const std::string jobDescr, const int & numParams)
{
  ListStrings jobInfos;
  boost::split(jobInfos, jobDescr, boost::is_any_of("@"));
  if(jobInfos.size() != numParams) {
    throw TMSVishnuException(ERRCODE_INVALID_PARAM, "Bad job description "+std::string(jobDescr)+ "\n"
                             "Expects "+vishnu::convertToString(numParams)+" parameters following the pattern param1@param2...");
  }
  return jobInfos;
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
void OpenNebulaServer::replaceEnvVariables(const char* scriptPath) {
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

  std::ofstream ofs(scriptPath);
  ofs << scriptContent;
  ofs.close();
}

/**
 * @brief return a VM template for KVM virtual machine
 * @param options submit options
 * @return
 */
std::string
OpenNebulaServer::getKvmTemplate(const TMS_Data::SubmitOptions& options)
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

  return boost::str(
        boost::format(
          "NAME=\"centos-64\""
          "CPU=\"1\""
          "VCPU=\"2\""
          "MEMORY=\"512\""
          "DISK = [ IMAGE = \"centos-64\","
          "DRIVER=\"qcow2\"]"
          "OS=["
          "ARCH=\"i686\","
          "ROOT=\"sda1\","
          "BOOT=\"hd,fd,cdrom,network\" ]"
          "NIC = [ NETWORK = \"FIXED-NET\" ]"
          "GRAPHICS = ["
          "TYPE    = \"vnc\","
          "LISTEN  = \"0.0.0.0\","
          "KEYMAP = \"fr\"]"
          "RAW=[ "
          "  TYPE=\"kvm\","
          "  DATA=\""
          "    <serial type='pty'><target port='0'/></serial>"
          "    <console type='pty'><target type='serial' port='0'/></console>\"]"
          "CONTEXT=["
          "  HOSTNAME=\"vm-$VMID\","
          "  NETWORK=\"YES\","
          "  ETH0_IP=\"$NIC[IP, NETWORK=\\\"FIXED-NET\\\"]\","
          "  ETH0_GATEWAY=\"192.168.122.1\","
          "  GATEWAY=\"192.168.122.1\","
          "  FILES=\"/opt/software/opennebula-4.4.1/share/scripts/centos-5/context/init.sh\","
          "  ETH0_DNS=\"8.8.8.8\","
          "  NETMASK=\"255.255.255.0\","
          "  TARGET=\"hdb\" "
          "]"));
}
