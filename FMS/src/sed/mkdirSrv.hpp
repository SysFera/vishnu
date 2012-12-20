
/**
 * \file mkdirSrv.hpp
 * This file contains the declaration of the vishnu mkdir server
 */
#ifndef MKDIRSRV_HPP
#define MKDIRSRV_HPP

#include "DIET_client.h"

/**
 * \brief The mkdir service profile description
 * \return the mkdir service profile
 */
//diet_profile_desc_t* getCreateDirProfile();
/**
 * \brief the mkdir solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveCreateDir(diet_profile_t* profile);

#endif
