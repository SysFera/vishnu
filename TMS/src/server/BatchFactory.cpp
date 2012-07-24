/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#include "SharedLibrary.hh"

#include <boost/format.hpp>
#include <iostream>

static int created=0;

/**
 * \brief Constructor
 */
BatchFactory::BatchFactory() {
  mbatchServer = NULL;
}


BatchServer*
loadPluginBatch(const char *name) {
  dadi::SharedLibrary *plugin(NULL);
  void *factory(NULL);
  BatchServer *instance(NULL);

  std::string libname = boost::str(boost::format("%1%%2%%3%")
                                   % dadi::SharedLibrary::prefix()
                                   % name
                                   % dadi::SharedLibrary::suffix());

  plugin = new dadi::SharedLibrary(libname);
  if (plugin->isLoaded()) {
    factory = plugin->symbol("create_plugin_instance");
    ((int (*)(void **))(factory))((void**) &instance);
  }

  return instance;
}



/**
 * \brief Function to create a batchServer.
 * \param batchType The type of batchServer to create
 * \return an instance of BatchServer
 */
BatchServer*
BatchFactory::getBatchServerInstance() {
  BatchServer *instance(NULL);
#ifdef HAVE_TORQUE_2_3
    instance = loadPluginBatch("vishnu-tms-torque2.3");
#elif HAVE_LOADLEVELER_2_5
    instance = loadPluginBatch("vishnu-tms-loadleveler2.5");
#elif HAVE_SLURM_2_2
      instance = loadPluginBatch("vishnu-tms-slurm2.2");
#elif HAVE_SLURM_2_3
      instance = loadPluginBatch("vishnu-tms-slurm2.3");
#elif HAVE_LSF_7_0
      instance = loadPluginBatch("vishnu-tms-lsf7.0");
#elif HAVE_SGE_11
      instance = loadPluginBatch("vishnu-tms-sge11");
#elif HAVE_PBSPRO_10_4
      instance = loadPluginBatch("vishnu-tms-pbspro10.4");
#endif

  return static_cast<BatchServer *>(instance);
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
