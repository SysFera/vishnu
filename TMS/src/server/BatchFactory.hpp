/**
 * \file BatchFactory.hpp
 * \brief This file implements the batch factory
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#ifndef TMS_BATCH_FACTORY_H 
#define TMS_BATCH_FACTORY_H

#include "utilVishnu.hpp"
#include "BatchServer.hpp"

/**
 * \class BatchFactory
 * \brief A factory class to manage the life of BatchServer instance
 */
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
     * \return an instance of BatchServer
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
