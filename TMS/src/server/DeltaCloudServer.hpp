/*
 * DeltaCloudServer.hpp
 *
 *  Created on: 27 nov. 2012
 *      Author: r
 */

#ifndef DELTACLOUDSERVER_HPP_
#define DELTACLOUDSERVER_HPP_
#include <string>
#include <vector>
#include "BatchServer.hpp"
#include "libdeltacloud/libdeltacloud.h"


class DeltaCloudServer : public BatchServer {
public:
	DeltaCloudServer();
	virtual ~DeltaCloudServer();

	/**
	 * \brief Function to submit a job
	 * \param scriptPath the path to the script containing the job characteristique
	 * \param options the options to submit job
	 * \param job The job data structure
	 * \param envp The list of environment variables used by submission function
	 * \return raises an exception on error
	 */
	int
	submit(const char* scriptPath,
			const TMS_Data::SubmitOptions& options,
			TMS_Data::Job& job,
			char** envp=NULL);

	/**
	 * \brief Function to cancel job
	 * \param jobDescr the description of the job in the form of jobId@vmId
	 * \return raises an exception on error
	 */
	int
	cancel(const char* jobDescr) ;

	/**
	 * \brief Function to get the status of the job
	 * \param jobDescr the job description along with the descritpion of the associated virtual machine
	 * \return -1 if the job is unknown or server not  unavailable
	 */
	int
	getJobState(const std::string& jobDescr);

	/**
	 * \brief Function to get the start time of the job
	 * \param jobId the identifier of the job
	 * \return 0 if the job is unknown
	 */
	time_t
	getJobStartTime(const std::string& jobId);


	/**
	 * \brief Function to request the status of queues
	 * \param optQueueName (optional) the name of the queue to request
	 * \return The requested status in to ListQueues data structure
	 */
	TMS_Data::ListQueues*
	listQueues(const std::string& optQueueName=std::string());

	/**
	 * \brief Function to get a list of submitted jobs
	 * \param listOfJobs the ListJobs structure to fill
	 * \param ignoredIds the list of job ids to ignore
	 */
	void fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
			const std::vector<std::string>& ignoredIds=std::vector<std::string>());

private:
	/**
	 * \brief Function for initializing the deltacloud API
	 * \param url: The Url to access the API of the cloud provider
	 * \param user: The user login to the cloud provider. Note: For OpenStack this needs to be in the form of "login+tenant"
	 * param password: The user password
	 * return: throw exception on error
	 */
	void initialize(char* cloudEndpoint,
			char* user,
			char* password);

	/**
	 * \brief Function for cleaning up virtual machine
	 * \param vmid The id of the virtual machine
	 */
	void releaseResources(const std::string & vmid);

	/**
	 * \brief Function for cleaning up a deltacloud params list
	 * \param: params The list of params
	 */
	void cleanUpParams(std::vector<deltacloud_create_parameter>& params);

	/**
	 * \brief Function for cleaning up the allocated dynamic data structure
	 */
	void cleanup();

	/**
	 * \brief pointer to the deltacloud api
	 */	
	deltacloud_api *mcloudApi;
};

#endif /* DELTACLOUDSERVER_HPP_ */
