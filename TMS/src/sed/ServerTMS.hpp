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
   * \brief To get the unique instance of the server
   */
  static ServerTMS*  getInstance();

  /**
   * \brief Function to select a server for automatic selection
   * \param sessionKey The session key
   * \param criterion The selection criterion
   * \return the machine id
   */
  static std::string
  selectMachine(const string& sessionKey, const TMS_Data::LoadCriterion_ptr & criterion);

  /**
   * \brief To get the unique instance of the database
   */
  Database* getDatabaseVishnu();

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
  getBatchType();

  /**
   * \brief To get the machine id of the TMS server
   * \return the machine id
   */
  std::string
  getMachineId();

  /**
   * \brief To get the slave binary directory
   * \return path to the binary tmsSlave
   */
  std::string
  getSlaveDirectory();

  /**
   * \brief To get the Default Batch Options
   * \return batch Default Options Vector
   */
  std::vector<std::string>
  getDefaultBatchOption();
  /**
   * \brief To initialize the TMS Server class
   * \param vishnuId The identifier of the vishnu instance
   * \param dbConfig  The configuration of the database
   * \param machineId the id of the machine
   * \param batchType the type of batch scheduler
   * \param slaveBinDir  the directory that contains the slave binary
   * \return raises an exception on error
   */
  int
  init(int vishnuId,
	   DbConfiguration dbConfig,
	   const std::string& machineId,
	   BatchType batchType,
	   const std::string& slaveBinDir,
	   const std::string& uriNamer);

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ServerTMS();

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
   * \brief Function to compute the load performance of a given machine
   * \param sessionKey The session key
   * \param pb the request profile
   * \param the criteria of (number of waiting jobs, running jobs and total jobs)
   */
  static long
  getMachineLoadPerformance(const string& sessionKey, const UMS_Data::Machine_ptr &machine, const TMS_Data::LoadCriterion_ptr & criterion);

  /**
   * \brief operator=
   */
  ServerTMS& operator=(const ServerTMS&);

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
   * \brief represents The batch Type
   */
  BatchType mbatchType;
  /**
   * \brief represents The batch Type
   */
  std::string mmachineId;
  /**
   * \brief Structure representing a profile description
   */
  //  diet_profile_desc_t* mprofile;
  /**
   * \brief Instance of Database
   */
  Database *mdatabaseVishnu;
  /**
   * \brief Directory containing the slave binary
   */
  std::string mslaveBinDir;
  /**
   * \brief  Default batch submittion Options
   */
  std::vector<std::string> mdefaultBatchOption;
};
#endif // SERVER
