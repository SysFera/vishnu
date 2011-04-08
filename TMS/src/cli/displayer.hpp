#include "api_tms.hpp"

/**
 * \brief To display the output of all job
 * \fn void displayJobProgress(TMS_Data::ListJobResults& j)
 * \param j: the list of jobs output to display
 */
void 
displayAllJobOutput(TMS_Data::ListJobResults& j);

/**
 * \brief To display the output of a job
 * \fn void displayJobProgress(TMS_Data::JobResult_ptr j)
 * \param out: the outputpath of the job
 * \param err: the errorpath of the job
 */
void 
displayJobOutput(TMS_Data::JobResult_ptr j);

/**
 * \brief To display the job info
 * \fn void displayJob(Job& j)
 * \param j: The job to display the info
 */
void 
displayJob(TMS_Data::Job_ptr& j);
