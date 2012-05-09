/**
 * \file rmdirSrv.hpp
 * This file contains the declaration of the vishnu remove directory server
 */
#ifndef RMDIRSRV_HPP
#define RMDIRSRV_HPP

#include "DIET_client.h"

/**
 * \brief The remove directory service profile description
 * \return the remove directory service profile
 */
//diet_profile_desc_t* getRemoveDirProfile();

/**
 * \brief the remove directory solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveRemoveDir(diet_profile_t* profile);

#endif
