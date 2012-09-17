/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#ifdef HAVE_TORQUE_2_3
#include "TorqueServer.hpp"
#endif
#ifdef HAVE_LOADLEVELER_3_5
#include "LLServer.hpp"
#endif
#ifdef HAVE_SLURM_2_2
#include "SlurmServer.hpp"
#endif
#ifdef HAVE_SLURM_2_3
#include "SlurmServer.hpp"
#endif
#ifdef HAVE_LSF_7_0
#include "LSFServer.hpp"
#endif
#ifdef HAVE_SGE_11
#include "SGEServer.hpp"
#include "SGEConfig.hpp"
#include <iostream>

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
BatchFactory::getBatchServerInstance() {

#ifdef HAVE_TORQUE_2_3
  mbatchServer = new TorqueServer();
#elif HAVE_LOADLEVELER_3_5
  mbatchServer = new LLServer();
#elif HAVE_SLURM_2_2
  mbatchServer = new SlurmServer();
#elif HAVE_SLURM_2_3
  mbatchServer = new SlurmServer();
#elif HAVE_LSF_7_0
  mbatchServer = new LSFServer();
#elif HAVE_SGE_11
  mbatchServer = new SGEServer();
#else
  mbatchServer = NULL;
#endif

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
