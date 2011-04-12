/**
 * \file TMSServer.hpp
 * \brief This file presents the implementation of the TMS server.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
*/

#ifndef _TMS_SERVER_H_
#define _TMS_SERVER_H_

#include <string>

#include "DIET_server.h"
#include "DbConfiguration.hpp"
#include "utilVishnu.hpp"

/**
 *  * \brief Number of service in TMS
 *   */
#define NB_SRV 2

static const char* SERVICES[NB_SRV] = {
  "jobSubmit_",
  "jobCancel_"
};

/**
 * \class TMSServer
 * \brief This class describes the g server
 */
class TMSServer {
public :

  /**
   * \brief To get the unique instance of the server
   */
  static TMSServer*  getInstance();

  /**
  * \brief To get the batchType
  * \return the type of the underlying batch scheduler
  */
  static BatchType
  getBatchType();

 /**
  * \brief To get the machine id of the TMS server
  * \return the machine id
  */
  static std::string
  getMachineId();

   /**
   * \brief To initialize the TMS Server class
   * \param vishnuId The identifier of the vishnu instance
   * \param dbConfig  The configuration of the database
   * \param machineId the id of the machine
   * \param batchType the type of batch scheduler
   */
  int
  init(int vishnuId, DbConfiguration dbConfig, std::string machineId, BatchType batchType);

  /**
   * \brief Destructor, raises an exception on error
   * \fn ~TMSServer()
   */
  ~TMSServer();

private :

  /**
   * \brief Constructor, private because class is singleton
   */
  TMSServer();

  /**
   * \brief operator=
   */
  TMSServer& operator=(const TMSServer&);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The singleton reference
   */
  static TMSServer *minstance;
  /**
  * \brief represents The batch Type
  */
  static BatchType mbatchType;
  /**
  * \brief represents The batch Type
  */
  static std::string mmachineId;
  /**
  * \brief Structure representing a profile description
  */
  diet_profile_desc_t* mprofile;
  /**
  * \brief Instance of Database
  */
  //static Database *mdatabaseVishnu;
};
#endif // SERVERg
