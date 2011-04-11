/**
* \file internalTMSAPI.hpp
* \brief This file presents the the implementation of the TMS internalAPI.
* \author Daouda Traore (daouda.traore@sysfera.com)
* \date April 
*/

#ifndef INTERNAL_TMS_API_H
#define INTERNAL_TMS_API_H


/*
 * SOLVE FUNCTIONS
 */

/* submitJob */
int
solve_submitJob(diet_profile_t* pb);

/* cancelJob */
int
solve_cancelJob(diet_profile_t* pb);
#endif
