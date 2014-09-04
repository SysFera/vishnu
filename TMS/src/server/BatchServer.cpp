/**
 * \file BatchServer.hpp
 * \brief This file contains the VISHNU BatchServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "tmsUtils.hpp"
#include <boost/format.hpp>

/**
 * \brief Constructor
 */
BatchServer::BatchServer() {
}

/**
 * \brief Destructor
 */
BatchServer::~BatchServer() {
    
}


/**
     * @brief Get instance of batch server
     * @param batchType The batch type
     * @param version The batch version
     * @return
     */
BatchServer*
BatchServer::getBatchServer(int batchType, const std::string& version)
{
  BatchFactory factory;
  BatchServer* batchServer = factory.getBatchServerInstance(batchType, version);

  if (! batchServer) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             boost::str(boost::format("getBatchServerInstance return NULL (batch: %1%, version: %2%)")
                                        % vishnu::convertBatchTypeToString(static_cast<BatchType>(batchType))
                                        % version));

  }
  return batchServer;
}
