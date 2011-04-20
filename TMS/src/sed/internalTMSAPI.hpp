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
solveSubmitJob(diet_profile_t* pb);

/* cancelJob */
int
solveCancelJob(diet_profile_t* pb);

/* listQueues */
int
solveListOfQueues(diet_profile_t* pb);

/* jobOutPutGetResult */
int
solveJobOutPutGetResult(diet_profile_t* pb);
#endif
