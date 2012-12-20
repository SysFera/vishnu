/**
 * \file headSrv.hpp
 * This file contains the declaration of the vishnu head of file server
 */

#ifndef HEADSRV_HPP
#define HEADSRV_HPP

#include "DIET_client.h"

/**
 * \brief The head of file service profile description
 * \return the head of file service profile
 */
//diet_profile_desc_t* getHeadProfile();

/**
 * \brief the head of file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int headFile(diet_profile_t* profile);

#endif
