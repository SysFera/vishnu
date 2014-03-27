/*
 * \file OpenNebulaServer.hpp
 * \brief This file contains the implementation of the delta cloud batch scheduler interface in vishnu
 * \author Rodrigue Chakode (rodrigue.chakode@sysfera.com)
 * \date November 2012
 */

#ifndef OpenNebulaServer_HPP_
#define OpenNebulaServer_HPP_
#include <string>
#include <vector>
#include <boost/format.hpp>
#include "BatchServer.hpp"
#include "utilVishnu.hpp"
#include "OneRPCManager.hpp"

/**
 * \class OpenNebulaServer
 * \brief The implementation of the delta cloud interfacage as a batch scheduler
 */
class OpenNebulaServer : public BatchServer {
public:
  OpenNebulaServer();
  virtual ~OpenNebulaServer();

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
         TMS_Data::ListJobs& jobSteps,
         char** envp=NULL);

  /**
   * \brief Function to cancel job:  just shutdown and destroy the related VM
   * \param vmId the VM ID
   * \return raises an exception on error
   */
  int
  cancel(const std::string& vmId) ;

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
   * @brief The path to the contextualization init script
   */
  std::string mcontextInitScript;

  /**
   * @brief The name of the virtual network to used for VM
   */
  std::string mvirtualNetwork;

  /**
   * @brief The gateway to assign to VM network
   */
  std::string mvirtualNetworkGateway;

  /**
   * @brief The DNS to assign to VM network
   */
  std::string mvirtualNetworkDns;

  /**
   * @brief The netmask to assign to VM network
   */
  std::string mvirtualNetworkMask;

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
   * \brief Function for cleaning up virtual machine
   * \param vmId The id of the virtual machine
   */
  void
  releaseResources(const std::string & vmId);

  /**
   * \brief Function to decompose job information
   * \param jobDescr The description of the job in the form of param1\@param2@...
   * \param numParams The number of expected parameters
   * \return ListStrings aka a vector of string parameters or throw exception on error
   */
  ListStrings
  getJobInfos(const std::string jobDescr, const int & numParams);

  /**
   * \brief To retrieve specific submission parameters
   * \param specificParams The string containing the list of parameters
   */
  void
  retrieveUserSpecificParams(const std::string& specificParams);

  /**
   * \brief Function to replace some environment varia*bles in a string
   * \param scriptContent The string content to modify
   */
  void replaceEnvVariables(const std::string& scriptPath);

  /**
   * @brief return a VM template for KVM virtual machine
   * @param options submit options
   * @return
   */
  std::string
  getKvmTemplate(const TMS_Data::SubmitOptions& options);

  /**
   * @brief Return a string as expected by OpenNebula API (username:password)
   * @return string
   */
  std::string
  getSessionString(void);

  /**
   * @brief Return the defaultValue if a given string is empty
   * @param value The input string
   * @param defaultValue The default value
   * @return string
   */
  std::string
  returnInputOrDefaultIfEmpty(const std::string& value, const std::string& defaultValue);


  /**
   * @brief Return the defaultValue if a given int value is <= 0
   * @param value The input
   * @param defaultValue The default value
   * @return int
   */
  int
  returnInputOrDefaultIfNegativeNull(int value, int defaultValue);
};

#endif /* OpenNebulaServer_HPP_ */
