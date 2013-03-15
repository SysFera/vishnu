/**
 * \file ListQueuesServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>

#include "utilVishnu.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "ListQueuesServer.hpp"


/**
 * \brief Constructor, raises an exception on error
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 */
ListQueuesServer::ListQueuesServer(const SessionServer& session,
                                   const std::string& machineId,
                                   const BatchType& batchType,
                                   const std::string& batchVersion,
                                   const std::string& option)
 : moption(option), mlistQueues(NULL)
{
   BatchFactory factory;
   mbatchServer = factory.getBatchServerInstance(batchType, batchVersion);
}

/**
 * \brief Function to list machines information
 * \return The pointer to the TMS_Data::ListQueues containing users information
 * \return raises an exception on error
 */
TMS_Data::ListQueues* ListQueuesServer::list()
{
  return mbatchServer->listQueues(moption);
}

/**
 * \brief Destructor, raises an exception on error
 */
ListQueuesServer::~ListQueuesServer()
{
}

#endif
