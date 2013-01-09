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
#include "Env.hpp"
#include "utilServer.hpp"


DeltaCloudServer::DeltaCloudServer()
: mcloudUser(""),
  mcloudUserPassword(""),
  mvmImageId(""),
  mvmFlavor(""),
  mvmUser(""),
  mvmUserKey(""),
  mnfsServer(""),
  mnfsMountPoint("") {}

DeltaCloudServer::~DeltaCloudServer() {
	deltacloud_free(mcloudApi);
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
DeltaCloudServer::submit(const char* scriptPath,
		const TMS_Data::SubmitOptions& options,
		TMS_Data::Job& job,
		char** envp) {

	retrieveSpecificParams(options.getSpecificParams()); // First set specific parameters
	initialize(); // Initialize Delatacloud API

	// Get configuration parameters
	// We first try to get parameters set specifically for the job
	//FIXME: possibly memory leak if Env::getVar through exception. We may need to catch that and free deltacloud API by calling finalize()
	if (mvmImageId.empty()) {
		mvmImageId = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE], false);
	}
	if (mvmFlavor.empty()) {
		mvmFlavor = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR], false);
	}
	if (mvmUser.empty()) {
		mvmUser = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], false);
	}
	if (mvmUserKey.empty()) {
		mvmUserKey = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], false);
	}
	if (mnfsServer.empty()) {
		mnfsServer = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_SERVER], false);
	}
	if(mnfsMountPoint.empty()) {
		mnfsMountPoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], false);
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

	param.name = strdup("user_data");
	param.value = strdup("day=kkkqkhq&month=hsqgdgqjdfd");
	params.push_back(param);

	std::string vmName = "vishnu.vm."+job.getJobId();
	param.name = strdup("name");
	param.value = strdup(vmName.c_str());
	params.push_back(param);

	char *instid = NULL;
	if (deltacloud_create_instance(mcloudApi, mvmImageId.c_str(), &params[0], params.size(), &instid) < 0) {
		cleanUpParams(params);
		finalize();
		if (instid == NULL) {
			throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
					std::string(deltacloud_get_last_error_string())+". Symptom: deltacloud_create_instance return NULL " );
		}
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				std::string(deltacloud_get_last_error_string())+"::deltacloud_create_instance");
	}
	cleanUpParams(params);

	struct deltacloud_instance instance;
	if(wait_for_instance_boot(mcloudApi, instid, &instance) != 1) {
		std::string msg = (boost::format("Instance never went RUNNING or went "
				"to a unexpected state %1%, failing")%instance.state).str();
		deltacloud_instance_destroy(mcloudApi, &instance);
		cleanUpParams(params);
		finalize();
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, msg);
	}

	//No Longer require
//	if (deltacloud_get_instance_by_id(mcloudApi, instid, &instance) < 0) {
//		cleanUpParams(params);
//		finalize();
//		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
//				std::string(deltacloud_get_last_error_string())+"::deltacloud_get_instance_by_id");
//	}
	std::clog << boost::format("[TMS][INFO] Virtual machine started\n"
			" ID: %1%\n"
			" NAME: %2%\n"
			" IP: %3%\n")%instance.id%instance.name%instance.private_addresses->address;
	// Create the NODEFILE
	// The path must correspond with the value set for the
	// environment variable VISHNU_BATCHJOB_NODEFILE in JobServer.cpp
	vishnu::saveInFile(job.getOutputDir()+"/NODEFILE", instance.private_addresses->address);

	// Create an ssh engine for the virtual machine
	// And submit the script
	SSHJobExec sshEngine(mvmUser, instance.private_addresses->address);
	int jobPid = -1;
	try {
		jobPid = sshEngine.execRemoteScript(scriptPath, mnfsServer, mnfsMountPoint, job.getOutputDir());
	} catch(...) {
		throw;
	}

	job.setBatchJobId(vishnu::convertToString(jobPid));
	job.setJobName(job.getBatchJobId());
	job.setJobId(vishnu::convertToString(jobPid));
	job.setVmId(instance.id);
	job.setStatus(vishnu::JOB_SUBMITTED);
	job.setVmIp(instance.private_addresses->address);
	job.setOutputPath(job.getOutputDir()+"/stdout");
	job.setErrorPath(job.getOutputDir()+"/stderr");
	job.setNbNodes(1);

	deltacloud_free_instance(&instance);
	finalize();
	return 0;
}

/**
 * \brief Function to cancel job
 * \param jobDescr the description of the job in the form of jobId@vmId
 * \return raises an exception on error
 */
int
DeltaCloudServer::cancel(const char* jobDescr) {

	ListStrings jobInfos = getJobInfos(jobDescr, 2);
	try {
		releaseResources(jobInfos[1]); // Stop the virtual machine to release resources
	} catch(const VishnuException & ex) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "[ERROR] Can not cancel the job " + jobInfos[0]);
	}
	return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobDescr the job description in the form of pid@user@vmaddress@vmId
 * \return -1 if the job is unknown or server not unavailable
 */
int
DeltaCloudServer::getJobState(const std::string& jobDescr) {

	// Get job infos
	ListStrings jobInfos = getJobInfos(jobDescr, 4);
	std::string pid = jobInfos[0];
	std::string vmUser = jobInfos[1];
	std::string vmIp = jobInfos[2];
	std::string vmId = jobInfos[3];

	SSHJobExec sshEngine(vmUser, vmIp);
	std::string statusFile = "/tmp/"+jobDescr;
	sshEngine.execCmd("ps -o pid= -p " + pid +" | wc -l >"+statusFile, false);

	int status = vishnu::JOB_RUNNING;
	int count = vishnu::getStatusValue(statusFile);
	if( count == 0) {
		// stop the virtual machine to release the resources
		releaseResources(vmId);
		status = vishnu::JOB_COMPLETED;
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
DeltaCloudServer::getJobStartTime(const std::string& jobDescr) {

	initialize(); // Initialize the Cloud API
	ListStrings jobInfos = getJobInfos(jobDescr, 2); // Get the job information
	deltacloud_instance instance; // Get the instance
	if (deltacloud_get_instance_by_id(mcloudApi, jobInfos[1].c_str(), &instance) < 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}
	vishnu::convertToTimeType(instance.launch_time);
	deltacloud_free_instance(&instance);
	finalize();
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
		const std::vector<std::string>& ignoredIds) {
	//TODO The semantic is no yet defined
}


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
		mcloudEndpoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
	}
	if(mcloudUser.empty()) {
		mcloudUser= Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
	}
	if(mcloudUserPassword.empty()) {
		mcloudUserPassword = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);
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
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}
	std::clog << boost::format("[TMS][INFO] The instance %1% (NAME: %2%) will be stopped")%instance.id%instance.name;
	if (deltacloud_instance_stop(mcloudApi, &instance) < 0) { // Stop the instance
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}

	deltacloud_free_instance(&instance);
	finalize();
}

/**
 * \brief Function for cleaning up a deltacloud params list
 * \param: jobDescr The description of the job in the form of param1@param2@...
 * \param: numParams The number of expected parameters
 */
ListStrings DeltaCloudServer::getJobInfos(const std::string jobDescr, const int & numParams) {

	ListStrings jobInfos;
	boost::split(jobInfos, jobDescr, boost::is_any_of("@"));
	if(jobInfos.size() != numParams) {
		throw TMSVishnuException(ERRCODE_INVALID_PARAM, "Bad job description "+std::string(jobDescr)+ "\n"
				"Expects "+vishnu::convertToString(numParams)+" parameters in the form of param1@param2...");
	}
	return jobInfos;
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
	for (ListStrings::iterator it = listParams.begin(); it != listParams.end(); it++) {
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
