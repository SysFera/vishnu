
/**
 * \file getContentSrv.hpp
 * This file contains the declaration of the vishnu get file content server
 */


#ifndef GETCONTENTSRV_HPP
#define GETCONTENTSRV_HPP

#include "DIET_server.h"

/**
 * \brief The get file content service profile description
 * \return the get file content service profile
 */
diet_profile_desc_t* getContentProfile();
/**
 * \brief the get file content solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int contentFile(diet_profile_t* profile);

#endif
