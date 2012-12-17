/*
 * DeltaCloudServer.cpp
 *
 *  Created on: 27 nov. 2012
 *      Author: r
 */

#include <boost/format.hpp>
#include "DeltaCloudServer.hpp"
#include "TMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#include "SSHJobExec.hpp"
#include "string.h"
#include "common.h"
#include <vector>
#include "constants.hpp"
#include <ctime>


DeltaCloudServer::DeltaCloudServer(){}

DeltaCloudServer::~DeltaCloudServer() {
	deltacloud_free(mcloudApi);
}

/**
 * \brief Function for initializing the deltacloud API
 * \param url: The Url to access the API of the cloud provider
 * \param user: The user login to the cloud provider. Note: For OpenStack this needs to be in the form of "login+tenant"
 * param password: The user password
 */
void DeltaCloudServer::initialize(char* cloudEndpoint,
		char* user,
		char* password) {

	mcloudApi =  new deltacloud_api;
	if (deltacloud_initialize(mcloudApi, cloudEndpoint, user, password) < 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}
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


	// Get the cloud endpoint from environment variables
	char* cloudEndpoint = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT].c_str());
	if(cloudEndpoint  == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No cloud user set. "
				"You may need to set the environment variable VISHNU_CLOUD_ENDPOINT" );
	}

	// Get the cloud user from environement variables
	char* cloudUser= getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER].c_str());
	if(cloudUser  == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No cloud user set. "
				"You may need to set the environment variable VISHNU_CLOUD_USER" );
	}

	// Get the virtual machine default user's password from environment variables
	char* cloudUserPassword = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD].c_str());
	if(cloudUserPassword == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No password set for the cloud user."
				"You may need to set the environment variable VISHNU_CLOUD_USER_PASSWORD" );
	}

	// Get the virtual machine image from environment variables
	char* imageId = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE].c_str());
	if(imageId == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No virtual machine image has been set. "
				"You may need to set the environment variable VISHNU_CLOUD_VM_IMAGE" );
	}

	// Get the default flavor of creating instances
	char* flavor = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR].c_str());
	if(flavor  == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No default flavor set. "
				"You may need to set the environment variable VISHNU_CLOUD_DEFAULT_FLAVOR");
	}

	// Get the virtual machine default user from environment variables
	char* vmUser = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER].c_str());
	if(vmUser == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No user has been set for accessing virtual machines. "
				"You may need to set the environment variable VISHNU_CLOUD_VM_USER" );
	}

	// Get the virtual machine default user's key from environment variables
	char* vmUserKey = getenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY].c_str());
	if(vmUserKey == NULL) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"No key has been set for accessing virtual machines. "
				"You may need to set the environment variable VISHNU_CLOUD_VM_USER_KEY" );
	}

	// NOW, Initialize the Delatacloud API
	initialize(cloudEndpoint, cloudUser, cloudUserPassword);

	// Set the parameters of the virtual machine instance
	std::vector<deltacloud_create_parameter> params;
	deltacloud_create_parameter param;
	param.name = strdup("hwp_id");
	param.value = strdup(flavor);
	params.push_back(param);

	param.name = strdup("keyname");
	param.value = strdup(vmUserKey);
	params.push_back(param);


	std::string vmName = "vishnu-"+vishnu::convertToString(time(NULL));
	param.name = strdup("name");
	param.value = strdup(vmName.c_str());
	params.push_back(param);

	char *instid = NULL;
	if (deltacloud_create_instance(mcloudApi, imageId, &params[0], 2, &instid) < 0){
		cleanUpParams(params);
		if (instid == NULL){
			throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string())+". Symptom: deltacloud_create_instance return NULL " );
		}
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()) + "::deltacloud_create_instance");
	}
	cleanUpParams(params);

	struct deltacloud_instance instance;
	if(wait_for_instance_boot(mcloudApi, instid, &instance) != 1) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()) + "::wait_for_instance_boot");
	}

	if (deltacloud_get_instance_by_id(mcloudApi, instid, &instance) < 0){
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()) + "::deltacloud_get_instance_by_id");
	}

	std::cout << ">>Virtual machine started"
			  << "\n  ID: "<< std::string(instance.id)
		      << "\n  Name: " << std::string(instance.name)
	          << "\n  IP: "<< std::string(instance.private_addresses->address)
	          << "\n";

	SSHJobExec sshEngine(vmUser, instance.private_addresses->address);
	int jobPid = -1;
	try{
		jobPid = sshEngine.execRemoteScript(scriptPath);
	} catch(...) {
		throw;
	}

	job.setBatchJobId(vishnu::convertToString(jobPid));
	job.setJobId(vishnu::convertToString(jobPid));
	job.setVmId(instance.id);
	job.setStatus(vishnu::JOB_COMPLETED);
	job.setVmIp(instance.private_addresses->address);
	cleanup();
	return 0;
}

/**
 * \brief Function to cancel job
 * \param jobId the identifier of the job to cancel
 * \return raises an exception on error
 */
int
DeltaCloudServer::cancel(const char* jobId) {
	//TODO

	return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job
 * \return -1 if the job is unknown or server not  unavailable
 */
int
DeltaCloudServer::getJobState(const std::string& jobId){
	//TODO

	return 1;
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job
 * \return 0 if the job is unknown
 */
time_t
DeltaCloudServer::getJobStartTime(const std::string& jobId) {
	//TODO
}


/**
 * \brief Function to request the status of queues
 * \param optQueueName (optional) the name of the queue to request
 * \return The requested status in to ListQueues data structure
 */
TMS_Data::ListQueues*
DeltaCloudServer::listQueues(const std::string& optQueueName) {
	//TODO
}


/**
 * \brief Function to get a list of submitted jobs
 * \param listOfJobs the ListJobs structure to fill
 * \param ignoredIds the list of job ids to ignore
 */
void DeltaCloudServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
		const std::vector<std::string>& ignoredIds) {
	//TODO
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
 * \brief Function for cleaning up the allocated dynamic data structure
 */
void DeltaCloudServer::cleanup() {
	deltacloud_free(mcloudApi);
	delete mcloudApi;
}

/**
 * \brief Function for cleaning up a deltacloud params list
 * \param: params The list of params
 */
void DeltaCloudServer::cleanUpParams(std::vector<deltacloud_create_parameter>& params){
	for(int i=0; i< params.size(); i++) {
		free(params[i].name);
		free(params[i].value);
	}
}
