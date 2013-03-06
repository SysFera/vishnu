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
#include "utilVishnu.hpp"


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
  DELTACLOUD = 6, /*!< For DELTACLOUD batch type */
  POSIX = 7, /*!< For POSIX batch type */
  UNDEFINED = 8 /*!< IF batch type is not defined*/
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

static const std::string AUTOMATIC_SUBMIT_JOB_KEYWORD="autom";
static const std::string LIST_JOBS_ON_MACHINES_KEYWORD="all";

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
 * \brief  function to convert job status into string
 * \param state: The state of job
 * \return The converted state value
 */
std::string
convertJobStateToString(const int& state);

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
 * \param IN opt A structure containing the set of submitted options
 * \param OUT paramStr a string containing all of parameters
 * \param IN paramOptName the name of the option for a single parameter
 * \param IN paramsVector a vector of parameters
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
 *  \param Id of the machine
 */
inline std::string getMachineName(const std::string& sessionKey, const std::string& machineId);

/**
 * \brief Function to copy a list of remote files to a local directory
 * \param srcMid : Id of the remote machine
 * \param rfiles : List of the files to copy
 * \param ldestDir : Destination directory on the local machine
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \param missingFiles: The list of missing files*
 * \param startPos : Position of the file
 * \return Throw exception on error
 */
void copyFiles(const std::string& sessionKey,
               const std::string& srcMid,
               const std::vector<std::string>& rfiles,
               const std::string& ldestDir,
               const FMS_Data::CpFileOptions& copts,
               std::string& missingFiles,
               const int& startPos=0);

/**
 * \brief Function to copy a remote file to a local directory
 * \param srcMachineId : Id of the source machine
 * \param srcPath : Path of the source file
 * \param destMachineId : Id of the destination machine
 * \param destPath : The destination path (may be a directory)
 * \param copts : Copy option (false => non recursive, 0 => scp)
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
 * \param srcFiles : String describing the source files
 * \param destMachineId : Id of the destination machine
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \return A string describing the destination file. The function throw exception on error
 */
std::string
sendInputFiles(const std::string& sessionKey,
               const std::string& srcFiles,
               const std::string& destMachineId,
               const FMS_Data::CpFileOptions& copts);

/**
 * \brief Function to select a machine for automatic submission
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \param The selection criterion
 * \return the selected machine or raises an exception on error
 */
std::string
findMachine(const std::string& sessionKey,
            const TMS_Data::LoadCriterion_ptr& criterion);

/**
 * \brief Function to compute the load performance of a given machine
 * \param sessionKey The session key
 * \param pb the request profile
 * \param the criteria of (number of waiting jobs, running jobs and total jobs)
 */
static long
getMachineLoadPerformance(const std::string& sessionKey,
                          const UMS_Data::Machine_ptr& machine,
                          const TMS_Data::LoadCriterion_ptr& criterion);


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
} //END NAMESPACE

#endif // TMSUTILS_HPP
