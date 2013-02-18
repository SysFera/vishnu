/**
 * \file ServerTMS.hpp
 * \brief This file presents the implementation of the TMS server.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#ifndef _TMS_SERVER_H_
#define _TMS_SERVER_H_

#include <string>

#include "DIET_client.h"
#include "DbConfiguration.hpp"
#include "utilVishnu.hpp"
#include "TMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "SeD.hpp"
#include "SessionServer.hpp"

class Database;

/**
 * \brief Number of service in TMS
 */
#define NB_SRV 11

static const char* SERVICES[NB_SRV] = {
  "jobSubmit",
  "jobCancel",
  "jobInfo",
  "getListOfJobs",
  "getJobsProgression",
  "getListOfQueues",
  "jobOutputGetResult",
  "jobOutputGetCompletedJobs",
  "getListOfJobs_all",
  "jobSubmit_autom",
  "addwork"
};

/**
 * \class ServerTMS
 * \brief This class describes the g server
 */
class ServerTMS : public SeD {
public :

  /**
     * \brief To initialize the TMS Server class
     * \param vishnuId The identifier of the vishnu instance
     * \param dbConfig  The configuration of the database
     * \param machineId the id of the machine
     * \param batchType The type of the batch scheduler*
     * \param batchType the type of batch scheduler
     * \param slaveBinDir  the directory that contains the slave binary
     * \return raises an exception on error
     */
  int
  init(int & vishnuId,
       DbConfiguration & dbConfig,
       const std::string & machineId,
       BatchType batchType,
       const std::string & slaveBinDir,
       const ExecConfiguration_Ptr sedConfig);

  /**
     * \brief Destructor, raises an exception on error
     */
  ~ServerTMS();

  /**
    * \brief To get the unique instance of the server
    */
  static ServerTMS*
  getInstance();

  /**
     * \brief To get the unique instance of the database
     */
  Database*
  getDatabaseVishnu();

  /**
     * \brief to get the VishnuId
     * \return the path of the configuration file
     */
  int
  getVishnuId() const;

  /**
     * \brief To get the batchType
     * \return the type of the underlying batch scheduler
     */
  BatchType
  getBatchType() const;

  /**
   * \brief To get the batchVersion
   * \return the version of the underlying batch scheduler
   */
  std::string
  getBatchVersion() const;

  /**
     * \brief To get the machine id of the TMS server
     * \return the machine id
     */
  std::string
  getMachineId() const;

  /**
     * \brief To get the slave binary directory
     * \return path to the binary tmsSlave
     */
  std::string
  getSlaveDirectory() const;

  /**
     * \brief To get the Default Batch Options
     * \return batch Default Options Vector
     */
  std::vector<std::string>
  getDefaultBatchOption() const;

  /**
     * \brief To initialize the TMS Server class
     * \param vishnuId The identifier of the vishnu instance
     * \param dbConfig  The configuration of the database
     * \param machineId the id of the machine
     * \param slaveBinDir  the directory that contains the slave binary
     * \param sedConfig A pointer to the SeD configuration
     * \return raises an exception on error
     */
  int
  init(int& vishnuId,
       DbConfiguration& dbConfig,
       const std::string& machineId,
       const std::string& slaveBinDir,
       const ExecConfiguration_Ptr sedConfig);


  /**
     * \brief To get the main configuration
     * \return the pointer to configuration object
     */
  ExecConfiguration_Ptr
  getSedConfig() const;

  /**
   * \brief Function to get the debug level
   */
  int getDebugLevel() const {return mdebugLevel;}

private :
  /**
     * \brief Init the ptr function map
     * \param mid The machine Id
     */
  void
  initMap(std::string mid);

  /**
     * \brief Constructor, private because class is singleton
     */
  ServerTMS();

  /**
     * \brief operator=
     */
  ServerTMS& operator=(const ServerTMS&);

  /**
     * \brief Function to get the default Batch submission options
     * \param configPath The job script path
     * \param defaultOptions The list of the option value
     * \return raises an exception on error
     */
  void
  getConfigOptions(const char* configPath,
                   std::vector<std::string>& defaultOptions,
                   const char* batchKey);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
     * \brief The singleton reference
     */
  static ServerTMS *minstance;

  /**
     * \brief Instance of TMSMapper
     */

  static TMSMapper *mmapper;
  /**
     * \brief Path to the file containing the namer uri
     */
  static std::string muriNamerCfg;

  /**
     * \brief The vishnu id
     */
  int mvishnuId;

  /**
   * \brief represents the batch Type
     */
  BatchType mbatchType;

  /**
   * \brief represents the batch Version
   */
  std::string mbatchVersion;
  /**
   * \brief represents the machine ID
     */
  std::string mmachineId;

  /**
     * \brief Instance of Database
     */
  Database* mdatabaseVishnu;

  /**
     * \brief Directory containing the slave binary
     */
  std::string mslaveBinDir;

  /**
    /**
    * \brief The configuration of the SeD
    */
  ExecConfiguration_Ptr msedConfig;

  /**
    * \brief  Default batch submittion Options
    */
  std::vector<std::string> mdefaultBatchOption;

  /**
   * \brief Holds the level of debug
   */
  int mdebugLevel;
};
#endif // SERVER
