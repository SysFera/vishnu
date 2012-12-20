
/**
 * \file chmodSrv.hpp
 * This file contains the declaration of the vishnu change mode server
 */

#ifndef CHMODSRV_HPP
#define CHMODSRV_HPP

#include "DIET_client.h"

/**
 * \brief The Change mode service profile description
 * \return the change mode service profile
 */
//diet_profile_desc_t* getChangeModeProfile();

/**
 * \brief the change mode solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int solveChangeMode(diet_profile_t* profile);

#endif
