/**
 * \file internalApiIMS.hpp
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
*/

#ifndef __INTERNALAPIIMS__HH__
#define __INTERNALAPIIMS__HH__

#include "DIET_server.h"


/**
* \brief Function to solve the service export
* \fn    int solveExport(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveExport(diet_profile_t* pb);
/**
* \brief Function to solve the service to get hte current metric
* \fn    int solveCurMetric(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveCurMetric(diet_profile_t* pb);
/**
* \brief Function to solve the service to get the metric history
* \fn    int solveOldMetric(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveOldMetric(diet_profile_t* pb);
/**
* \brief Function to solve the service to get the processes
* \fn    int solvePS(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solvePS(diet_profile_t* pb);
/**
* \brief Function to solve the service to set the system info
* \fn    int solveSetSysInfo(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetSysInfo(diet_profile_t* pb);
/**
* \brief Function to solve the service to set a threshold
* \fn    int solveSetThreshold(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetThreshold(diet_profile_t* pb);
/**
* \brief Function to solve the service get a threshold
* \fn    int solveGetThreshold(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveGetThreshold(diet_profile_t* pb);
/**
* \brief Function to solve the service to set the user id format
* \fn    int solveSetUID(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetUID(diet_profile_t* pb);
/**
* \brief Function to solve the service to set the job id format
* \fn    int solveSetJID(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetJID(diet_profile_t* pb);
/**
* \brief Function to solve the service to the the transfer id format
* \fn    int solveSetTID(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetTID(diet_profile_t* pb);
/**
* \brief Function to solve the service to set the machine id format
* \fn    int solveSetMID(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetMID(diet_profile_t* pb);
/**
* \brief Function to solve the service to load shed
* \fn    int solveLoadShed(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveLoadShed(diet_profile_t* pb);
/**
* \brief Function to solve the service to set the update frequency
* \fn    int solveSetUpFreq(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveSetUpFreq(diet_profile_t* pb);
/**
* \brief Function to solve the service export
* \fn    int solveGetUpFreq(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveGetUpFreq(diet_profile_t* pb);
/**
* \brief Function to solve the service export
* \fn    int solveRestart(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveRestart(diet_profile_t* pb);
/**
* \brief Function to solve the service export
* \fn    int solveStop(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveStop(diet_profile_t* pb);
/**
* \brief Function to solve the service export
* \fn    int solveGetSysInfo(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveGetSysInfo(diet_profile_t* pb);

#endif // Internal api IMS
