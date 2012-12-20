
/**
 * \file getInfosSrv.hpp
 * This file contains the declaration of the vishnu get infos server
 */

#ifndef GETINFOSSRV_HPP
#define GETINFOSSRV_HPP

#include "DIET_client.h"

/**
 * \brief The get infos service profile description
 * \return the change group service profile
 */
//diet_profile_t* getInfosProfile();
/**
 * \brief the get infos solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int get_infos(diet_profile_t* profile);


#endif
