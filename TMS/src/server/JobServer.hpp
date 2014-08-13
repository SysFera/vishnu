/**
  * \file JobServer.hpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_SERVER_H
#define _JOB_SERVER_H

#include "BatchServer.hpp"
#include "utils.hpp"
#include <string>
#include "TMS_Data.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "tmsUtils.hpp"


/**
 * \class JobServer
 * \brief JobServer class implementation
 */
class JobServer {
public:

  enum BacthExecModeT {
    SshExecMode = 0,
    NativeExecMode = 1
  };

  enum BacthActionT {
    SubmitBatchAction = 0,
    CancelBatchAction = 1
  };

  /**
  * \param authKey The session info
  * \param machineId The machine identifier
  * \param jsonJob an JsonObject describing the job
  * \param sedConfig A pointer to the SeD configuration
  * \brief Constructor
  */
  explicit JobServer(const std::string& authKey,
                     const std::string& machineId,
                     const ExecConfiguration_Ptr sedConfig);


  /**
   * \brief Function to submit job
   * \param scriptContent the content of the script
   * \param options a json object describing options
   * \param defaultBatchOption the default options on the batch scheduler
   * \return The resulting job ID. Raises an exception on error
   */
  std::string
  submitJob(std::string& scriptContent,
            JsonObject* options,
            const std::vector<std::string>& defaultBatchOption);

  /**
   * \brief Destructor
   */
  ~JobServer();

  /**
   * \brief Function to cancel job
   * \param options Object containing options
   * \return raises an exception on error
   */
  int
  cancelJob(JsonObject* options);

  /**
   * \brief Function to get job information
   * \param jobId The id of the job
   * \return The job data structure
   */
  TMS_Data::Job
  getJobInfo(const std::string& jobId);

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

  /**
   * @brief Return true if the backend is a cloud
   * @return
   */
  static bool
  isCloudBackend(int batchType) {return batchType == DELTACLOUD || batchType == OPENNEBULA;}

private:
  /**
   * \brief Check the machineid is correct
   * \param machineId the machineId to check
   */
  void
  checkMachineId(std::string machineId);
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
   * \param defaultBatchOption The list of the option value
   * \param content The content of the option
   * \param key The key for the option
   * \return raises an exception on error
   */
  void
  processDefaultOptions(const std::vector<std::string>& defaultBatchOption, std::string& content, std::string& key);

  /**
   * \brief Function to insert option line into string
   * \param optionLineToInsert the option to insert
   * \param content The buffer containing the inserted option
   * \param key The key to insert
   * \return raises an exception on error
   */
  void
  insertOptionLine(std::string& optionLineToInsert, std::string& content, std::string& key);

  /*
   * \brief Return the directive associated to the batch scheduler
   * \param seperator Hold the seperator used to define parameter
   * \return the directive associated to the batch scheduler
   */
  std::string
  getBatchDirective(std::string& seperator) const;

  /**
   * \brief Set specific parameters for job submission
   * \param specificParams The string containing the list of parameters
   * \param scriptContent The content of the script when required
   */
  void
  handleSpecificParams(const std::string& specificParams,
                       std::string& scriptContent);

  /**
   * @brief Update the result job steps with the base information of the job and save them
   * @param jobSteps The list of steps
   * @param baseJobInfo The base job info
   */
  void
  saveJobSteps(TMS_Data::ListJobs& jobSteps, TMS_Data::Job& defaultJobInfo);

  /**
   * \brief Function to save the encapsulated job into the database
   * @param action The type of action to finalize (submit, cancel...)
   * @param job The concerned job
   */
  void
  dbSave(int action, TMS_Data::Job& job);

  /**
   * \brief Function to set the Working Directory
   * \param scriptContent The script content
   * \param options a json object describing options
   * \param jobInfo The job information, could be altered with output path
   */
  void
  setRealFilePaths(std::string& scriptContent,
                   JsonObject* options,
                   TMS_Data::Job& jobInfo);

  /**
   * \brief Function to process the script with options
   * \param scriptContent the script content
   * \param options a JsonObject containing options
   * \param defaultBatchOption The default batch options
   * \param machineName the name of the machine
   * \return the processed script content
   */
  std::string
  processScript(std::string& scriptContent,
                JsonObject* options,
                const std::vector<std::string>& defaultBatchOption,
                const std::string& machineName);

  /**
   * @brief Submit job using ssh mechanism
   * @param action action The type of action (cancel, submit...)
   * @param scriptPath The path of the script to executed
   * @param baseJobInfo The base job info
   * @param options: an object containing options
   * @param batchType The batch type
   * @param batchVersion The batch version. Ignored for POSIX backend
  */
  void
  handleSshBatchExec(int action,
                     const std::string& scriptPath,
                     JsonObject* options,
                     TMS_Data::Job& baseJobInfo,
                     int batchType,
                     const std::string& batchVersion);

  /**
   * @brief Submit job using ssh mechanism
   * @param action action The type of action (cancel, submit...)
   * @param scriptPath The path of the script to executed
   * @param options: an object containing options
   * @param jobInfo The default information provided to the job
   * @param batchType The batch type
   * @param batchVersion The batch version. Ignored for POSIX backend
  */
  void
  handleNativeBatchExec(int action,
                        const std::string& scriptPath,
                        JsonObject* options,
                        TMS_Data::Job& jobInfo,
                        int batchType,
                        const std::string& batchVersion);

  /**
   * @brief Get the uid corresponding to given system user name
   * @param username
   * @return
   */
  uid_t
  getSystemUid(const std::string& name);

  /**
   * @brief create the executable for submit job script
   * @param content The script content
   * @param options The submit options
   * @param defaultBatchOption The default batch options
   * @return The script path
   */
  std::string
  createJobScriptExecutaleFile(std::string& content,
                               JsonObject* options,
                               const std::vector<std::string>& defaultBatchOption);

  /**
   * @brief Export environment variables used throughout the execution, notably in cloud mode
   * @param defaultJobInfo The default job info
   */
  void
  exportJobEnvironments(const TMS_Data::Job &defaultJobInfo);

  /**
   * @brief Check whether the logged user is an admin
   * @return
   */
  bool
  isAdminSession(void) {return muserSessionInfo.user_privilege == vishnu::PRIVILEGE_ADMIN;}

private:

  /**
   * @brief Session info
   */
  std::string mauthKey;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;

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
  Database* mdatabase;

  /**
  * \brief The configuration of the SeD
  */
  ExecConfiguration_Ptr msedConfig;

  /**
   * @brief mstandaloneSed
   */
  int mstandaloneSed;

  /**
   * \brief Holds the level of debug
   */
  int mdebugLevel;
};

#endif
