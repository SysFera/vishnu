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

/**
 * \brief Function to solve the service solveGenerique
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveJobInfo(diet_profile_t* pb);

/**
* \brief Function to solve the service solveGenerique
* \fn    int solveGenerique(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb);

/* getListOfJobs */
int
solveGetListOfJobs(diet_profile_t* pb);

/* listJobsProgression */
int
solveGetListOfJobsProgression(diet_profile_t* pb);

/* listQueues */
int
solveListOfQueues(diet_profile_t* pb);

/* jobOutPutGetResult */
int
solveJobOutPutGetResult(diet_profile_t* pb);
#endif
