
/**
 * \file lsSrv.hpp
 * This file contains the declaration of the vishnu list directory server
 */

#ifndef LSSRV_HPP
#define LSSRV_HPP

#include "DIET_client.h"

/**
 * \brief The list directory service profile description
 * \return the list directory service profile
 */
//diet_profile_desc_t* getListDirProfile();

/**
 * \brief the list directory solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveListDir(diet_profile_t* profile);

#endif
