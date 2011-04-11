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
displayJob(TMS_Data::Job& j);

/**
 * \brief To display the job info in a list
 * \fn void displayJobProgress(ListProgression& j)
 * \param j: The progression to display the info
 */
void 
displayJobProgress(ListProgression& j);

/**
 * \brief To display the job info
 * \fn void displayJobProgress(Progression& j)
 * \param j: The progression to display the info
 */
void
displayProgress(Progression& j);

/**
 * \brief To display the refresh period
 * \fn void displayRefreshPeriod(int& j)
 * \param j: The refresh period
 */
void 
displayRefreshPeriod(int& j);

/**
 * \brief To display the list of jobs
 * \fn void displayListJobs(ListJobs& p)
 * \param j: The list of job to display 
 */
void 
displayListJobs(ListJobs& j);

/**
 * \brief To display the queues info
 * \fn void displayQueues(ListQueues& j)
 * \param j: The list of queues to display the info
 */
void 
displayQueues(ListQueues& j);


/**
 * \brief To display the info about a queue
 * \fn void displayQueues(ListQueues& j)
 * \param j: The list of queues to display the info
 */
void
displayQueue(Queue& j);
