#include "api_tms.hpp"

/**
 * \brief To display the output of all job
 * \param j: the list of jobs output to display
 */
void 
displayAllJobOutput(TMS_Data::ListJobResults& j);

/**
 * \brief To display the output of a job
 * \param j: the job output result
 */
void 
displayJobOutput(TMS_Data::JobResult_ptr j);

/**
 * \brief To display the job info
 * \param j: The job to display the info
 */
void 
displayJob(TMS_Data::Job& j);

/**
 * \brief To display the job info in a list
 * \param j: The progression to display the info
 */
void 
displayJobProgress(ListProgression& j);

/**
 * \brief To display the job info
 * \param j: The progression to display the info
 */
void
displayProgress(Progression& j);

/**
 * \brief To display the list of jobs
 * \param j: The list of job to display 
 */
void 
displayListJobs(ListJobs& j);

/**
 * \brief To display the queues info
 * \param j: The list of queues to display the info
 */
void 
displayQueues(ListQueues& j);


/**
 * \brief To display the info about a queue
 * \param j: The list of queues to display the info
 */
void
displayQueue(Queue& j);

/**
 * \brief To display some basic job info after submit
 * \param job: The job to submit data
 */
void
displaySubmit(TMS_Data::Job job);

/**
 * \brief  function to convert job status into string 
 * \param state: The state of job
 * \return The converted state value
 */
std::string convertJobStateToString(const int& state);

/**
 * \brief  function to convert job priority into string 
 * \param state: The state of job
 * \return The converted state value
 */
std::string convertJobPriorityToString(const int& prio);

/**
 * \brief Helper function to display a list of queues
 * \param os: The output stream in which the list will be printed 
 * \param lsQueues: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListQueues& lsQueues);

/**
 * \brief Helper function to display a list of jobs 
 * \param os: The output stream in which the list will be printed 
 * \param ListJobs: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListJobs& listJobs);

/**
 * \brief Helper function to display a list of jobs progression
 * \param os: The output stream in which the list will be printed 
 * \param listProgress: The list to display
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, ListProgression& listProgress);

