
/**
 * \file tailSrv.hpp
 * This file contains the declaration of the vishnu tail of file server
 */

#ifndef TAILSRV_HPP
#define TAILSRV_HPP

#include "DIET_client.h"

/**
 * \brief The tail of file service profile description
 * \return the tail of file service profile
 */
//diet_profile_desc_t* getTailProfile();
/**
 * \brief the tail of file solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */
int tailFile(diet_profile_t* profile);

#endif
