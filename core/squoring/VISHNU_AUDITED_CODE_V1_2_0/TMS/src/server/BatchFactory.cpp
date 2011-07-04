/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#ifdef HAVE_TORQUE
#include "TorqueServer.hpp"
#endif
#ifdef HAVE_LOADLEVELER
#include "LLServer.hpp"
#endif


/**
 * \brief Constructor
 */
BatchFactory::BatchFactory() {
  mbatchServer = NULL;
}

/**
 * \brief Function to create a batchServer.
 * \param batchType The type of batchServer to create
 * \return an instance of BatchServer
 */
BatchServer*
BatchFactory::getBatchServerInstance(BatchType batchType) {

  switch (batchType){
    case TORQUE:
#ifdef HAVE_TORQUE
      mbatchServer = new TorqueServer();
#else
      mbatchServer = NULL;
#endif
      break;
    case LOADLEVELER:
#ifdef HAVE_LOADLEVELER
      mbatchServer = new LLServer();
#else
      mbatchServer = NULL;
#endif
      break;
    default:
      mbatchServer = NULL;
  }

  return mbatchServer;
}

/**
 * \brief Function to free a batchServer instance.
 */
void BatchFactory::freeBatchServerInstance() {
   delete mbatchServer;
}

/**
 * \brief Destructor
 */
BatchFactory::~BatchFactory() {

}
