/**
 * \file ListProgressServer.hpp
 * \brief This file contains the VISHNU ListProgressServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_PROGRESS_SERVER_
#define _LIST_PROGRESS_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "QueryServer.hpp"
#include "SessionServer.hpp"
#include "TMS_Data.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"

/**
 * \class ListProgressServer
 * \brief ListProgressServer class implementation
 */
class ListProgressServer: public QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression> {

public:

  /**
   * \fn ListProgressServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param machineId The identifier of the machine on which the jobs whill be listed  
   * \brief Constructor, raises an exception on error
   */
  ListProgressServer(const SessionServer session, const std::string& machineId):
    QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(session)
  {
   mcommandName = "vishnu_get_job_progress";
   mmachineId = machineId;
  }
  /**
   * \fn ListProgressServer(const UMS_Data::ListSessionOptions_ptr params,
   *                        const SessionServer& session)
   * \param params The object which encapsulates the information of ListProgressServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListProgressServer(TMS_Data::ProgressOptions_ptr params, const SessionServer& session, const std::string& machineId):
    QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(params, session)
  {
    mcommandName = "vishnu_get_job_progress";
    mmachineId = machineId;
  }

  /**
   * \brief Function to list sessions information
   * \fn UMS_Data::ListSessions* list()
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListProgression*
  list() {
    
    BatchFactory factory;
    BatchType batchType  = ServerTMS::getInstance()->getBatchType();
    BatchServer* batchServer = factory.getBatchServerInstance(batchType);

    mlistObject = batchServer->getJobProgress(*mparameters);
    return mlistObject;

  }


  /**
   * \brief Function to get the name of the ListProgressServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListProgressServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListProgressServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListProgressServer()
  {
  }

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListProgressServer command line
  */
  std::string mcommandName;
   /**
  * \brief The identifier of the machine in which the jobs whill be listed
  */
  std::string mmachineId; 
};

#endif
