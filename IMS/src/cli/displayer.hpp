#include "api_ims.hpp"

/**
 * \brief To display the list of processes
 * \param li: The list of processes to display
 */
void
displayListProc(IMS_Data::ListProcesses* li);

/**
 * \brief To display a process
 * \param p: The process to display
 */
void
displayProc(IMS_Data::Process* p);

/**
 * \brief To display the list of metrics
 * \param li: The list of metrics to display
 */
void
displayListMetric(IMS_Data::ListMetric* li);

/**
 * \brief To display a metric
 * \param p: The metric to display
 */
void
displayMetric(IMS_Data::Metric* p);
/**
 * \brief To display the update frequency
 * \param freq: The frequency to display
 */
void
displayFreq(int freq);

/**
* \brief To display a system information
* \param sysInfo: The system information to display
*/
void
displaySystemInfo(IMS_Data::SystemInfo* sysInfo);

/**
 * \brief To display the list of system information
 * \param li: The list of system information to display
 */
void
displayListSysInfo(IMS_Data::ListSysInfo* li);

/**
 * \brief  function to convert the type of metric into string
 * \param metricType: The type of metric
 * \return The converted state value
 */
std::string
convertMetricToString(const int& metricType);

/**
* \brief To display a system threshold
* \param systemThreshold: The system threshold to display
*/
void
displaySystemThreshold(IMS_Data::Threshold* systemThreshold);

/**
 * \brief To display the list of system threshold
 * \param li: The list of system threshold to display
 */
void
displayListSysThreshold(IMS_Data::ListThreshold* li);
