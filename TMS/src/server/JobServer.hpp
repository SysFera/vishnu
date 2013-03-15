/**
  * \file JobServer.hpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_SERVER_H
#define _JOB_SERVER_H

#include "TMS_Data.hpp"
#include <string>
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "tmsUtils.hpp"

/**
 * \class JobServer
 * \brief JobServer class implementation
 */
class JobServer {
public:
  /**
  * \param sessionServer The object which encapsulates the session information
  * \param machineId The machine identifier
  * \param job The job data structure
  * \param sedConfig A pointer to the SeD configuration
  * \brief Constructor
  */
  explicit JobServer(const SessionServer& sessionServer,
                     const std::string& machineId,
                     const TMS_Data::Job& job,
                     const ExecConfiguration_Ptr sedConfig);


  /**
   * \brief Function to submit job
   * \param scriptContent the content of the script
   * \param options the options to submit job
   * \param vishnuId The VISHNU identifier
   * \return raises an exception on error
   */
  int submitJob(std::string& scriptContent,
                TMS_Data::SubmitOptions& options,
                const int& vishnuId,
                const std::vector<std::string>& defaultBatchOption);

  /**
   * \brief Destructor
   */
  ~JobServer();

  /**
   * \brief Function to cancel job
   * \return raises an exception on error
   */
  int
  cancelJob();

  /**
   * \brief Function to get job information
   * \return The job data structure
   */
  TMS_Data::Job
  getJobInfo();

  /**
   * \brief Function to get job data
   * \return The job data structure
   */
  TMS_Data::Job
  getData();

  /**
   * \brief To set the main configuration
   */
  void
  setSedConfig(ExecConfiguration_Ptr sedConfig);

  /**
   * \brief To get the main configuration
   * \return the pointer to configuration object
   */
  ExecConfiguration_Ptr
  getSedConfig() const;

  /**
   * \brief Set the debug level
   * \param debugLevel The debug level
   */
  void
  setDebugLevel(const int& debugLevel) { mdebugLevel = debugLevel; }

protected:

  /**
   * \brief Function to scan VISHNU error message
   * \param errorInfo the error information to scan
   * \param code The code The code of the error
   * \param message The message associeted to the error code
   * \return raises an exception on erroor
   */
  void
  scanErrorMessage(const std::string& errorInfo, int& code, std::string& message);

  /**
   * \brief Function to convert a given date into correspondant long value
   * \param date The date to convert
   * \return The converted value
   */
  long long
  convertToTimeType(std::string date);

  /**
   * \brief Function to treat the default submission options
   * \param scriptOptions The list of the option value
   * \param cmdsOptions The list of the option value
   * \return raises an exception on error
   */
  void
  processDefaultOptions(const std::vector<std::string>& defaultBatchOption, std::string& content, std::string& key);

  /**
   * \brief Function to insert option line into string
   * \param optionLineToInsert the option to insert
   * \param content The buffer containing the inserted option
   * \return raises an exception on error
   */
  void
  insertOptionLine(std::string& optionLineToInsert, std::string& content, std::string& key);

  /**
  * \brief Function to set the path of output directory
  * \param parentDir The directory in which to create the output dir
  * \param dirSuffix the suffix of the output dir
  * \param content the script content to be update which the generated path
  */
  void
  computeOutputDir(const std::string& parentDir,
                   const std::string & dirSuffix,
                   std::string & content);

  /*
   * \brief Return the directive associated to the batch scheduler
   * \param seperator Hold the seperator used to define parameter
   */
  std::string
  getBatchDirective(std::string& seperator) const;

  /**
   * \brief Set specific parameters for job submission
   * \param specificParams The string containing the list of parameters
   * \param scriptContent The content of the script when required
   */
  void
  treatSpecificParams(const std::string& specificParams,
                      std::string& scriptContent);


  /**
   * \brief Function to save the encapsulated job into the database
   */
  void
  recordJob2db();

  /**
   * \brief Function to get the hostname of a machine id
   * \param machineId Id of the machine
   * \return The machine name or throw exception on error
   */
  std::string
  getMachineName(const std::string& machineId);

  /**
   * \brief Function to set the Working Directory
   * \param options the options to submit job
   * \param suffix the suffix of the working directory
   * \return the script path
   */
  std::string
  computeWorkingDir(TMS_Data::SubmitOptions& options,
                    const std::string& suffix);
  /**
   * \brief Function to process the script with options
   * \param the script content
   * \param options the options to submit job
   * \param defaultBatchOption The default batch options
   * \return the processed script content
   */
  std::string
  processScript(std::string& scriptContent,
                TMS_Data::SubmitOptions& options,
                const std::vector<std::string>& defaultBatchOption,
                const std::string& machineName);
  /**
   * \brief Function to deserialize job
   * \param jobSerialized the Serialized job
   */
  void
  deserializeJob(std::string jobSerialized);

private:
  /**
   * \brief job data structure
   */
  TMS_Data::Job mjob;

  /**
   * \brief The object which encapsulates the session information
   */
  SessionServer msessionServer;

  /**
   * \brief The machine identifier
   */
  std::string mmachineId;

  /**
   * \brief The type of the batch scheduler
   */
  BatchType mbatchType;

  /**
   * \brief The type of the batch scheduler
   */
  std::string mbatchVersion;

  /**
   * \brief An instance of vishnu database
   */
  Database *mdatabaseVishnu;

  /**
  * \brief The configuration of the SeD
  */
  ExecConfiguration_Ptr msedConfig;

  /**
   * \brief Holds the level of debug
   */
  int mdebugLevel;
};

#endif
