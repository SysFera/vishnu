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
 * \fn BatchFactory()
 */
BatchFactory::BatchFactory() {

}

/**
 * \brief Function to create a batchServer. The batchServer created depends on the type parameter
 * \param batchType The type of batchServer to create
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
 * \brief Destructor
 * \fn ~BatchFactory()
 */
BatchFactory::~BatchFactory() {

}
