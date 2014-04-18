/*
 * tmsClientUtils.cpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */
#ifndef TMSUTILS_HPP
#define TMSUTILS_HPP

#include <string>
#include "FMS_Data/CpFileOptions.hpp"
#include "TMS_Data/LoadCriterion.hpp"
#include "UMS_Data/Machine.hpp"
#include "Options.hpp"
#include "utilVishnu.hpp"
#include <boost/assign/list_of.hpp>
#include "constants.hpp"


/**
 * \enum BatchType
 * \brief The type of the Batch
 */
typedef enum {
  TORQUE = 0,/*!< For TORQUE batch type */
  LOADLEVELER = 1,/*!< For LOADLEVELER batch type */
  SLURM = 2, /*!< For SLURM batch type */
  LSF = 3, /*!< For LSF batch type */
  SGE = 4, /*!< For SGE batch type */
  PBSPRO = 5, /*!< For PBS Pro batch type */
  DELTACLOUD = 6, /*!< DeltaCloud backend */
  POSIX = 7, /*!< For POSIX batch type */
  OPENNEBULA = 8, /*!< OpenNebula backend */
  UNDEFINED = 99 /*!< IF batch type is not defined*/
} BatchType;

/**
 * \enum LoadType
 * \brief The type of the scheduling criteria
 */
typedef enum {
  NBWAITINGJOBS = 0,
  NBJOBS = 1,
  NBRUNNINGJOBS = 2
} LoadType;

static const std::string AUTOM_KEYWORD="autom";
static const std::string ALL_KEYWORD="all";


const std::map<std::string, int> BATCH_NAME_TO_TYPE_MAP = boost::assign::map_list_of
                                                          ("TORQUE", TORQUE)
                                                          ("LOADLEVELER", LOADLEVELER)
                                                          ("SLURM", SLURM)
                                                          ("LSF", LSF)
                                                          ("SGE", SGE)
                                                          ("DELTACLOUD", DELTACLOUD)
                                                          ("OPENNEBULA", PBSPRO)
                                                          ("POSIX", POSIX);

const std::map<int, std::string> JOB_STATE_TO_NAME_MAP = boost::assign::map_list_of
                                                         (vishnu::STATE_SUBMITTED, "SUBMITTED")
                                                         (vishnu::STATE_QUEUED, "QUEUED")
                                                         (vishnu::STATE_WAITING, "WAITING")
                                                         (vishnu::STATE_RUNNING, "RUNNING")
                                                         (vishnu::STATE_COMPLETED, "COMPLETED")
                                                         (vishnu::STATE_CANCELLED, "CANCELLED")
                                                         (vishnu::STATE_DOWNLOADED, "DOWNLOADED")
                                                         (vishnu::STATE_FAILED, "FAILED")
                                                         (vishnu::STATE_UNDEFINED, "UNDEFINED");


const std::set<std::string> TORQUE_SUPPORTED_VERSION = boost::assign::list_of
                                                       ("2.3");
const std::set<std::string> PBS_SUPPORTED_VERSION = boost::assign::list_of
                                                    ("10.4");
const std::set<std::string> LSF_SUPPORTED_VERSION = boost::assign::list_of
                                                    ("7.0");
const std::set<std::string> SGE_SUPPORTED_VERSION = boost::assign::list_of
                                                    ("11");
const std::set<std::string> LOADLEVELER_SUPPORTED_VERSION = boost::assign::list_of
                                                            ("2.x")
                                                            ("3.x");
const std::set<std::string> SLURM_SUPPORTED_VERSION = boost::assign::list_of
                                                      ("2.2")
                                                      ("2.3")
                                                      ("2.4")
                                                      ("2.5")
                                                      ("2.6");
const std::set<std::string> DELTACLOUD_SUPPORTED_VERSION = boost::assign::list_of
                                                           ("ANY");
const std::set<std::string> OPENNEBULA_SUPPORTED_VERSION = boost::assign::list_of
                                                           ("ANY");


const std::map<int, std::set<std::string> > SUPPORTED_BATCH_VERSIONS = boost::assign::map_list_of
                                                                       (TORQUE, TORQUE_SUPPORTED_VERSION)
                                                                       (PBSPRO, PBS_SUPPORTED_VERSION)
                                                                       (LSF, LSF_SUPPORTED_VERSION)
                                                                       (SGE, SGE_SUPPORTED_VERSION)
                                                                       (DELTACLOUD, DELTACLOUD_SUPPORTED_VERSION)
                                                                       (OPENNEBULA, OPENNEBULA_SUPPORTED_VERSION)
                                                                       (LOADLEVELER, LOADLEVELER_SUPPORTED_VERSION)
                                                                       (SLURM, SLURM_SUPPORTED_VERSION);
namespace vishnu
{
  /**
 * \brief Function to convert a string to a batch type
 * \param  batchName the name of the batch type
 * \return BatchType value of the corresponding string.
 */
  BatchType
  convertToBatchType(const std::string& batchName);

  /**
     * \brief Function to convert the batch type to string
     * \param batchType the batch type to convert
     * \return the converted batch type
     */
  std::string convertBatchTypeToString(BatchType batchType);

  /**
   * \brief  function to convert job status into string
   * \param state: The state of job
   * \return The converted state value
   */
  std::string
  convertJobStateToString(int state);

  /**
   * \brief Function a given walltime into string
   * \param walltime The walltime to convert
   * \return the walltime converted to string
   */
  std::string
  convertWallTimeToString(const long& walltime);


  /**
   * \brief Function a given walltime into seconds
   *
   * walltime can be of the following format:
   * - it can start and/or end by '"'
   * - each value is separated by ':'
   * - possible values represent:
   *   - days
   *   - hours
   *   - minutes
   *   - seconds
   * in the form [days:][hours:][minutes:]seconds
   *  (element between square brackets are optional)
   *
   * \param walltime The walltime to convert
   * \return the walltime converted to seconds
   */
  long
  convertStringToWallTime(const std::string& walltime);


  /**
   * \brief Function to check the job status
   * \param status the status of the job
   * \return raises an exception on error
   */
  void
  checkJobStatus(const int& status);

  /**
   * \brief Function to check the job priority
   * \param priority the priority of the job
   * \return raises an exception on error
   */
  void
  checkJobPriority(const int& priority);

  /**
   * \brief Function to check the job nbNodesAndCpuPerNode
   * \param nbNodesAndCpuPerNode the number of nodes and cpu per node
   * \return raises an exception on error
   */
  void
  checkJobNbNodesAndNbCpuPerNode(const std::string& nbNodesAndCpuPerNode);

  /**
 * \brief Function to parse textual or file parameters
 * \param opt A structure containing the set of submitted options
 * \param paramsStr a string containing all of parameters
 * \param paramOptName the name of the option for a single parameter
 * \param paramsVector a vector of parameters
 * \return true if all parameters are syntaxicaly valid
 */
  int
  validateParameters(const boost::shared_ptr<Options> & opt,
                     std::string & paramsStr,
                     const std::string & paramOptName,
                     const std::vector<std::string> & paramsVector);


  /**
 * \brief Function to get the list of output files related to a job
 * \param result : The Job Result
 * \param appendJobId : Determine whether or not append the job id before the files lists
 * \return The list of files
 * Throw exception on error
 * */
  std::string
  getResultFiles(const TMS_Data::JobResult & result,
                 const bool & appendJobId);

  /**
 * \brief Function to create a directory
 * \param dirPath The path of the directory
 * Throw exception on error
 * */
  void
  createOutputDir(std::string& dirPath);

  /**
 * \brief Function to get the hostname of a machine id
 * \param sessionKey The session key
 * \param machineId Id of the machine
 * \return the name of the machine
 */
  inline std::string getMachineName(const std::string& sessionKey, const std::string& machineId);


  /**
   * \brief Function to copy a list of remote files to a local directory
   * \param srcMid: Id of the remote machine
   * \param remoteFileList: List of the files to copy
   * \param localDestinationDir: Destination directory on the local machine
   * \param copts: Copy option (false => non recursive, 0 => scp)
   * \param missingFiles: The list of missing files
   * \param startPos: Position of the file
   * \return Throw exception on error
   */
  void
  copyFiles(const std::string& sessionKey,
            const std::string& sourceMachineId,
            const std::vector<std::string>& remoteFileList,
            const std::string& localDestinationDir,
            const FMS_Data::CpFileOptions& copts,
            std::string& missingFiles,
            const int& startPos);

  /**
 * \brief Function to copy a remote file to a local directory
 * \param sessionKey the session key
 * \param srcMachineId Id of the source machine
 * \param srcPath Path of the source file
 * \param destMachineId Id of the destination machine
 * \param destPath The destination path (may be a directory)
 * \param copts Copy option (false => non recursive, 0 => scp)
 * \return The copied file or throw exception on error
 */
  std::string
  genericFileCopier(const std::string& sessionKey,
                    const std::string& srcMachineId,
                    const std::string& srcPath,
                    const std::string& destMachineId,
                    const std::string& destPath,
                    const FMS_Data::CpFileOptions& copts);


  /**
 * \brief Function to copy a remote file to a local directory
 * \param sessionKey the session key
 * \param srcFiles String describing the source files
 * \param destMachineId Id of the destination machine
 * \param copts Copy option (false => non recursive, 0 => scp)
 * \return A string describing the destination file. The function throw exception on error
 */
  std::string
  sendInputFiles(const std::string& sessionKey,
                 const std::string& srcFiles,
                 const std::string& destMachineId,
                 const FMS_Data::CpFileOptions& copts);

  /**
 * @brief listMachinesWithUserLocalAccount
 * @param sessionKey
 * @param machines
 */
  void
  listMachinesWithUserLocalAccount(const std::string& sessionKey, UMS_Data::ListMachines& machines);

  /**
 * \brief Function to select a machine for automatic submission
 * \param sessionKey the session key
 * \param criterion The selection criterion
 * \return the selected machine or raises an exception on error
 */
  std::string
  findMachine(const std::string& sessionKey,
              const TMS_Data::LoadCriterion& criterion);


  /**
 * \brief Function to compute the load performance of a given machine
 * \param sessionKey The session key
 * \param pb the request profile
 * \param the criteria of (number of waiting jobs, running jobs and total jobs)
 */
  static long
  getMachineLoadPerformance(const std::string& sessionKey,
                            const UMS_Data::Machine_ptr& machine,
                            const TMS_Data::LoadCriterion& criterion);



  /**
   * \brief Function to create temporary file
   * \param fileName The name of the file to create
   * \param content The content of the file
   */
  void
  saveInFile(const std::string & fileName, const std::string& content);

  /**
   * \brief Function to create temporary file
   * \param fileName The name of the file to create
   * \param missingDesc The content of the file
   */
  void
  recordMissingFiles(const std::string & fileName, const std::string& missingDesc);

  /**
   * \brief Function to get the current binary directory
   */
  std::string
  getCurrentBinaryDir();

  /**
   * \brief Function to replace all occurences in a string
   * \param scriptContent The string to modify
   * \param oldValue The value to replace
   * \param newValue The new value
   */
  void
  replaceAllOccurences(std::string& scriptContent,
                       const std::string& oldValue,
                       const std::string& newValue);

  /**

   * \brief function to set parameters appearing in a script
   * \param scriptContent The string to modify
   * \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
   */
  void
  setParams(std::string& scriptContent, const std::string& params);

  /**
   * \brief Function to set environment variables accordinf to parameters
   * \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
   */
  void
  setParamsEnvVars(const std::string& params);

  /**
   * \brief Function to retrieve an environment variable
   * \param name The name of the variable
   * \param optional tell whether the parameter is optional or not
   * \param defaultValue give the default value return when the variable is optional
   * \return the value of the environment variable or throw exception is the variable is set and is not optional
   */
  std::string
  getVar(const std::string& name,
         const bool & optional = true,
         const std::string defaultValue = "");

  /**
   * @brief Get the home directory of the currently logged user
   * @return  A string
   */
  std::string
  getCurrentUserHome(void);

  /**
   * @brief copy a given file to the current user home
   * @param path The file path
   * @return The resulting file path
   */
  std::string
  copyFileToUserHome(const std::string& path);

  /**
   * @brief Check a version of a given batch type is supported
   * @param btype The batch type
   * @param version The given version
   * @param supportedVersion If case of error return supported batch version
   * @return True on success, false otherwise
   */
  bool
  checkIfSupportedBatchVersion(BatchType btype, const std::string& version, std::string& supportedVersion);

} //END NAMESPACE

#endif // TMSUTILS_HPP
