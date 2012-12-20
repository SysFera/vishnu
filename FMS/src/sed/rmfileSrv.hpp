
/**
 * \file rmfileSrv.hpp
 * This file contains the declaration of the vishnu remove file server
 */
#ifndef RMFILESRV_HPP
#define RMFILESRV_HPP

#include "DIET_client.h"

/**
 * \brief The remove file service profile description
 * \return the remove file service profile
 */
//diet_profile_desc_t* getRemoveFileProfile();
/**
 * \brief the remove file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveRemoveFile(diet_profile_t* profile);

#endif
