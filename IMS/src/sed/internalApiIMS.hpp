/**
 * \file internalApiIMS.hpp
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef __INTERNALAPIIMS__HH__
#define __INTERNALAPIIMS__HH__

#include "DIET_client.h"


/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveExport(diet_profile_t* pb);
/**
 * \brief Function to solve the service to get hte current metric
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveCurMetric(diet_profile_t* pb);
/**
 * \brief Function to solve the service to get the metric history
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveOldMetric(diet_profile_t* pb);
/**
 * \brief Function to solve the service to get the processes
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solvePS(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the system info
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetSysInfo(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set a threshold
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetThreshold(diet_profile_t* pb);
/**
 * \brief Function to solve the service get a threshold
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetThreshold(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the user id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetUID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the job id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetJID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to the the transfer id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetTID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the machine id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetMID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the authsystem id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetAID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the work id format
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetWID(diet_profile_t* pb);
/**
 * \brief Function to solve the service to load shed
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveLoadShed(diet_profile_t* pb);
/**
 * \brief Function to solve the service to set the update frequency
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSetUpFreq(diet_profile_t* pb);
/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetUpFreq(diet_profile_t* pb);
/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveRestart(diet_profile_t* pb);
/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveStop(diet_profile_t* pb);
/**
 * \brief Function to solve the service export
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetSysInfo(diet_profile_t* pb);

#endif // Internal api IMS
