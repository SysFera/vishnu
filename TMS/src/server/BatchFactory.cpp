/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#include "SharedLibrary.hh"

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
  void *factory(NULL);
  BatchServer *instance(NULL);
  dadi::SharedLibrary *plugin(NULL);
#ifdef HAVE_TORQUE_2_3
      plugin = new dadi::SharedLibrary("tms-server-torque2.3");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_LOADLEVELER_2_5
      plugin = new dadi::SharedLibrary("tms-server-loadleveler2.5");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_SLURM_2_2
      plugin = new dadi::SharedLibrary("tms-server-slurm2.2");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_SLURM_2_3
      plugin = new dadi::SharedLibrary("tms-server-slurm2.3");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_LSF_7_0
      plugin = new dadi::SharedLibrary("tms-server-lsf7.0");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_SGE_11
      plugin = new dadi::SharedLibrary("tms-server-sge11");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#elif HAVE_PBSPRO_10_4
      plugin = new dadi::SharedLibrary("tms-server-pbspro10.4");
      if (plugin->isLoaded()) {
        factory = plugin->symbol("create_plugin_instance");
        ((int (*)(void **))(factory))((void**) &instance);
      }
#else
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
