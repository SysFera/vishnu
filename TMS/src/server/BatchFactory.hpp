/**
 * \file BatchFactory.cpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#ifndef TMS_BATCH_FACTORY_H 
#define TMS_BATCH_FACTORY_H

#include "utilVishnu.hpp"
#include "BatchServer.hpp"

class BatchFactory
{

  public :
    /**
     * \brief Constructor
     */
    BatchFactory();
   
    /**
     * \brief Destructor
     */
    ~BatchFactory();
   
    /**
     * \brief Function to create a batchServer. 
     * \param batchType The type of batchServer to create
     */
    BatchServer*
    getBatchServerInstance(BatchType batchType);

  private :
    /**
     * \brief an instance of the batchServer
     */
    BatchServer* mbatchServer;

};

#endif
