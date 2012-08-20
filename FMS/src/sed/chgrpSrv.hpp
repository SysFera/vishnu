/**
 * \file chgrpSrv.hpp
 * This file contains the declaration of the vishnu change group server
 */
#ifndef CHGRPSRV_HPP
#define CHGRPSRV_HPP

#include "DIET_server.h"
/**
 * \brief The Change group service profile description
 * \return the change group service profile
 */
diet_profile_desc_t* getChangeGroupProfile();
/**
 * \brief The Change owner service profile description
 * \return the change owner service profile
 */
diet_profile_desc_t* getChangeOwnerProfile();

/**
 * \brief the change group solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */

int solveChangeGroup(diet_profile_t* profile);

/**
 * \brief the change owner solve function
 * \param profile the service profile
 * \return 0 if the service succeds or an error code otherwise
 */

int solveChangeOwner(diet_profile_t* profile);

#endif
