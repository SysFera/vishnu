/*
 * \file DeltaCloudServer.hpp
 * \brief This file contains the implementation of the delta cloud batch scheduler interface in vishnu
 * \author Rodrigue Chakode (rodrigue.chakode@sysfera.com)
 * \date November 2012
 */

#ifndef DELTACLOUDSERVER_HPP_
#define DELTACLOUDSERVER_HPP_
#include <string>
#include <vector>
#include "BatchServer.hpp"
#include "utilVishnu.hpp"
#include "libdeltacloud/libdeltacloud.h"

/**
 * \class DeltaCloudServer
 * \brief The implementation of the delta cloud interfacage as a batch scheduler
 */
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
  submit(const std::string& scriptPath,
      const TMS_Data::SubmitOptions& options,
      TMS_Data::Job& job,
      char** envp=NULL);

  /**
   * \brief Function to cancel job
   * \param jobDescr the description of the job in the form of jobId@vmId
   * \return raises an exception on error
   */
  int
  cancel(const std::string& jobDescr) ;

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
   * \brief pointer to the deltacloud api
   */
  deltacloud_api *mcloudApi;

  /**
   * \brief Holds the endpoint of the cloud infrastructure
   */
  std::string mcloudEndpoint;

  /**
   * \brief Holds the cloud user
   */
  std::string mcloudUser;

  /**
   * \brief Holds the cloud user passsword
   */
  std::string mcloudUserPassword;

  /**
   * \brief Holds the image id
   */
  std::string mvmImageId;

  /**
   * \brief Holds the instances' flavor
   */
  std::string mvmFlavor;

  /**
   * \brief Holds the virtual machine user
   */
  std::string mvmUser;

  /**
   * \brief Holds the virtual machine keyname
   */
  std::string mvmUserKey;

  /**
   * \brief Holds the address of the NFS server
   */
  std::string mnfsServer;

  /**
   * \brief Holds the NFS mount point
   */
  std::string mnfsMountPoint;

  /**
   * \brief Function for initializing the deltacloud API
   * return: throw exception on error
   */
  void
  initialize(void);

  /**
   * \brief Function for cleaning up the allocated dynamic data structure
   */
  void
  finalize();

  /**
   * \brief Function for cleaning up virtual machine
   * \param vmid The id of the virtual machine
   */
  void
  releaseResources(const std::string & vmid);

  /**
   * \brief Function for cleaning up a deltacloud params list
   * \param jobDescr The description of the job in the form of param1\@param2@...
   * \param numParams The number of expected parameters
   * \return ListStrings aka a vector of string parameters or throw exception on error
   */
  ListStrings
  getJobInfos(const std::string jobDescr, const int & numParams);

  /**
   * \brief Function for cleaning up a deltacloud params list
   * \param params The list of params
   */
  void
  cleanUpParams(std::vector<deltacloud_create_parameter>& params);

  /**
   * \brief To retrieve specific submission parameters
   * \param specificParams The string containing the list of parameters
   */
  void
  retrieveSpecificParams(const std::string& specificParams);

  /**
   * \brief Function to replace some environment varia*bles in a string
   * \param scriptContent The string content to modify
   */
  void replaceEnvVariables(const char* scriptPath);
};

#endif /* DELTACLOUDSERVER_HPP_ */
