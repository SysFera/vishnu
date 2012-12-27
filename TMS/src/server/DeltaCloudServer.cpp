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


DeltaCloudServer::DeltaCloudServer(){}

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

	// Get configuration parameters
	std::string cloudEndpoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
	std::string cloudUser= Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
	std::string cloudUserPassword = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);
	std::string imageId = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_IMAGE], false);
	std::string flavor = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_DEFAULT_FLAVOR], false);
	std::string vmUser = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], false);
	std::string vmUserKey = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER_KEY], false);
	std::string nfsServer = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_SERVER], false);
	std::string nfsMountPoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_NFS_MOUNT_POINT], false);

	// NOW, Initialize the Delatacloud API
	initialize(const_cast<char*>(cloudEndpoint.c_str()),
			const_cast<char*>(cloudUser.c_str()),
			const_cast<char*>(cloudUserPassword.c_str()));

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
	if (deltacloud_create_instance(mcloudApi, imageId.c_str(), &params[0], params.size(), &instid) < 0){
		cleanUpParams(params);
		if (instid == NULL){
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

	if (deltacloud_get_instance_by_id(mcloudApi, instid, &instance) < 0){
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				std::string(deltacloud_get_last_error_string())+"::deltacloud_get_instance_by_id");
	}

	std::clog << "[TMS][INFO] Virtual machine started\n"
			<< "  ID: "<< std::string(instance.id)<<"\n"
			<< "  NAME: " << std::string(instance.name)<<"\n"
			<< "  IP: "<< std::string(instance.private_addresses->address)<<"\n";

	//Create the nodefile
	// The path should correspond with the value set for the environment variable ISHNU_BATCHJOB_NODEFILE in JobServer.cpp
	vishnu::saveInFile(job.getOutputDir()+"/NODEFILE", instance.private_addresses->address);

	// Create an ssh engine for the virtual machine
	SSHJobExec sshEngine(vmUser, instance.private_addresses->address);
	// Execute the script
	int jobPid = -1;
	try{
		jobPid = sshEngine.execRemoteScript(scriptPath, nfsServer, nfsMountPoint, job.getOutputDir());
	} catch(...) {
		throw;
	}

	job.setBatchJobId(vishnu::convertToString(jobPid));
	job.setJobId(vishnu::convertToString(jobPid));
	job.setVmId(instance.id);
	job.setStatus(vishnu::JOB_SUBMITTED);
	job.setVmIp(instance.private_addresses->address);
	job.setOutputPath(job.getOutputDir()+"/stdout");
	job.setErrorPath(job.getOutputDir()+"/stderr");
	job.setNbNodes(1);
	cleanup();
	return 0;
}

/**
 * \brief Function to cancel job
 * \param jobDescr the description of the job in the form of jobId@vmId
 * \return raises an exception on error
 */
int
DeltaCloudServer::cancel(const char* jobDescr) {
	std::vector<std::string> jobInfos;
	boost::split(jobInfos, jobDescr, boost::is_any_of("@"));
	if(jobInfos.size() != 2) {
		throw TMSVishnuException(ERRCODE_INVALID_PARAM, "Bad job description "+std::string(jobDescr));
	}

	int ret = -1;
	try {
		releaseResources(jobInfos[1]);
	} catch(const VishnuException & ex) {
		std::clog << "[TMS][ERROR] Can not cancel the job " <<  jobInfos[0] << "\n";
	}

	return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobDescr the job description in the form of pid@user@vmaddress@vmId
 * \return -1 if the job is unknown or server not unavailable
 */
int
DeltaCloudServer::getJobState(const std::string& jobDescr){

	std::vector<std::string> jobInfos;
	boost::split(jobInfos, jobDescr, boost::is_any_of("@"));

	if(jobInfos.size() != 4) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"Bad job identifier: "+ jobDescr
				+".This would be in the form pid@user@vmaddress@vmId");
	}

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
		//release the resources allocated to the vm and mark the job completed
		releaseResources(vmId);
		status = vishnu::JOB_COMPLETED;
	}

	vishnu::deleteFile(statusFile.c_str());

	return status;
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
 * \brief Function for cleaning up virtual machine
 * \param vmid The id of the virtual machine
 */
void DeltaCloudServer::releaseResources(const std::string & vmid) {

	// Get the credentials to authenticate against the cloud
	std::string cloudEndpoint = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT], false);
	std::string cloudUser= Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER], false);
	std::string cloudUserPassword = Env::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER_PASSWORD], false);

	// Initialize the Cloud API
	initialize(const_cast<char*>(cloudEndpoint.c_str()),
			const_cast<char*>(cloudUser.c_str()),
			const_cast<char*>(cloudUserPassword.c_str()));

	deltacloud_instance instance;
	if (deltacloud_get_instance_by_id(mcloudApi, vmid.c_str(), &instance) < 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}

	std::clog << "[TMS][INFO] The instance "<< instance.id << " (NAME: "<< instance.name<<") will be stopped\n";
	if (deltacloud_instance_stop(mcloudApi, &instance) < 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(deltacloud_get_last_error_string()));
	}

	cleanup();
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
