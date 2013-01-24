/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include "BatchFactory.hpp"
#include <string>
#include <boost/format.hpp>
#include <iostream>

#include "SharedLibrary.hh"


static int created = 0;

/**
 * \brief Constructor
 */
BatchFactory::BatchFactory() {
  mbatchServer = NULL;
}


BatchServer*
loadPluginBatch(const char *name) {
  dadi::SharedLibrary *plugin(NULL);
  BatchServer *instance(NULL);

  std::string libname = boost::str(boost::format("%1%%2%%3%")
                                   % dadi::SharedLibrary::prefix()
                                   % name
                                   % dadi::SharedLibrary::suffix());

  plugin = new dadi::SharedLibrary(libname);
  if (plugin->isLoaded()) {
    void *factory = plugin->symbol("create_plugin_instance");
    reinterpret_cast<factory_function>(factory)(reinterpret_cast<void**>(&instance));
  }

  return instance;
}



// /**
//  * \brief Function to create a batchServer.
//  * \param batchType The type of batchServer to create
//  * \return an instance of BatchServer
//  */
// BatchServer*
// BatchFactory::getBatchServerInstance() {
//   BatchServer *instance(NULL);
// #ifdef HAVE_TORQUE_2_3
//     instance = loadPluginBatch("vishnu-tms-torque2.3");
// #elif HAVE_LOADLEVELER_2_5
//     instance = loadPluginBatch("vishnu-tms-loadleveler2.5");
// #elif HAVE_SLURM_2_2
//       instance = loadPluginBatch("vishnu-tms-slurm2.2");
// #elif HAVE_SLURM_2_3
//       instance = loadPluginBatch("vishnu-tms-slurm2.3");
// #elif HAVE_LSF_7_0
//       instance = loadPluginBatch("vishnu-tms-lsf7.0");
// #elif HAVE_SGE_11
//       instance = loadPluginBatch("vishnu-tms-sge11");
// #elif HAVE_PBSPRO_10_4
//       instance = loadPluginBatch("vishnu-tms-pbspro10.4");
// #elif HAVE_TMSPOSIX
//       instance = loadPluginBatch("vishnu-tms-posix1.0");
// #endif

//   return static_cast<BatchServer *>(instance);
// }

/**
 * \brief Function to create a batchServer.
 * \param batchType The type of batchServer to create
 * \param batchVersion The version of batchServer to create
 * \return an instance of BatchServer, or NULL
 */
BatchServer*
BatchFactory::getBatchServerInstance(int batchType,
                                     const std::string &batchVersion) {
  BatchServer *instance(NULL);
  std::string libname = "vishnu-tms-";
  std::string realBatchVersion = batchVersion;

  switch(batchType){
  case TORQUE:
    libname += "torque";
    break;
  case LOADLEVELER:
    libname += "loadleveler";
    break;
  case SLURM:
    libname += "slurm";
    break;
  case LSF:
    libname += "lsf";
    break;
  case SGE:
    libname += "sge";
    break;
  case PBSPRO:
    libname += "pbspro";
    break;
  case POSIX:
    libname += "posix";
    break;
  default : // Always compile tmp-posix
    libname += "posix";
    realBatchVersion = "1.0";
    break;
  }

  libname += realBatchVersion;
  instance = loadPluginBatch(libname.c_str());
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
