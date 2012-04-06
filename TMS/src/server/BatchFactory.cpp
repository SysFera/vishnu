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
#ifdef HAVE_SLURM
#include "SlurmServer.hpp"
#endif
#ifdef HAVE_LSF
#include "LSFServer.hpp"
#endif
#ifdef HAVE_SGE
#include "SGEServer.hpp"
#include "SGEConfig.hpp"
extern "C" {
#include "drmaa.h"
}
#endif
#include <iostream>

static int created=0;

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
    case SLURM:
#ifdef HAVE_SLURM
      mbatchServer = new SlurmServer();
#else
      mbatchServer = NULL;
#endif
      break;
    case LSF:
#ifdef HAVE_LSF
      mbatchServer = new LSFServer();
#else
      mbatchServer = NULL;
#endif
      break;
    case SGE:
#ifdef HAVE_SGE
      mbatchServer = new SGEServer();   
#else
      mbatchServer = NULL;
#endif
      break;      
    default:
      mbatchServer = NULL;
      break;
  }

  return mbatchServer;
}

/**
 * \brief Function to delete a batchServer.
 */
void BatchFactory::deleteBatchServerInstance() {
   delete mbatchServer;
}
/**
 * \brief Destructor
 */
BatchFactory::~BatchFactory() {
}
