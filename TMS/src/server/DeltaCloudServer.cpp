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


DeltaCloudServer::DeltaCloudServer() {}

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

	initialize(); // Initialize Delatacloud API

	// Get configuration parameters
	// At each time, we first try to get parameters set specifically for the job
	std::string envVarPrefix = job.getJobId()+"_";
	std::string imageId = Env::getVar(envVarPrefix+vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE], true);
	if (imageId.empty()) {
		imageId = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE], false);
	}
	std::string flavor = Env::getVar(envVarPrefix+vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR], true);
	if (flavor.empty()) {
		flavor = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR], false);
	}
	std::string vmUser = Env::getVar(envVarPrefix+vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], true);
	if (vmUser.empty()) {
		vmUser = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], false);
	}
	std::string vmUserKey = Env::getVar(envVarPrefix+vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], true);
	if (vmUserKey.empty()) {
		vmUserKey = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], false);
	}
	std::string nfsServer = Env::getVar(envVarPrefix+vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_SERVER], true);
	if (nfsServer.empty()) {
		nfsServer = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_SERVER], false);
	}
	std::string nfsMountPoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], true);
    if(nfsMountPoint.empty()) {
    	Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], false);
    }
	// Set the parameters of the virtual machine instance
	std::vector<deltacloud_create_parameter> params;
	deltacloud_create_parameter param;
	param.name = strdup("hwp_id");
	param.value = strdup(flavor.c_str());
	params.push_back(param);

	param.name = strdup("keyname");
	param.value = strdup(vmUserKey.c_str());
	params.push_back(param);

	param.name = strdup("user_data");
	param.value = strdup("day=kkkqkhq&month=hsqgdgqjdfd");
	params.push_back(param);

	std::string vmName = "vishnu.vm."+job.getJobId();
	param.name = strdup("name");
	param.value = strdup(vmName.c_str());
	params.push_back(param);

	char *instid = NULL;
	if (deltacloud_create_instance(mcloudApi, imageId.c_str(), &params[0], params.size(), &instid) < 0) {
		cleanUpParams(params);
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
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				std::string(deltacloud_get_last_error_string())+"::wait_for_instance_boot");
	}

	if (deltacloud_get_instance_by_id(mcloudApi, instid, &instance) < 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				std::string(deltacloud_get_last_error_string())+"::deltacloud_get_instance_by_id");
	}
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
	SSHJobExec sshEngine(vmUser, instance.private_addresses->address);
	int jobPid = -1;
	try {
		jobPid = sshEngine.execRemoteScript(scriptPath, nfsServer, nfsMountPoint, job.getOutputDir());
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

	std::string cloudEndpoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
	std::string cloudUser= Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
	std::string cloudUserPassword = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);

	mcloudApi =  new deltacloud_api;
	if (deltacloud_initialize(mcloudApi,
			const_cast<char*>(cloudEndpoint.c_str()),
			const_cast<char*>(cloudUser.c_str()),
			const_cast<char*>(cloudUserPassword.c_str())) < 0) {
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