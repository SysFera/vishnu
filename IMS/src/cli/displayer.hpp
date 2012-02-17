#include "api_ims.hpp"
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/**
 * \brief To display the list of processes
 * \param li: The list of processes to display
 */
VISHNU_API_LIB void
displayListProc(IMS_Data::ListProcesses* li);

/**
 * \brief To display a process
 * \param p: The process to display
 */
VISHNU_API_LIB void
displayProc(IMS_Data::Process* p);

/**
 * \brief To display the list of metrics
 * \param li: The list of metrics to display
 */
VISHNU_API_LIB void
displayListMetric(IMS_Data::ListMetric* li);

/**
 * \brief To display a metric
 * \param p: The metric to display
 */
VISHNU_API_LIB void
displayMetric(IMS_Data::Metric* p);
/**
 * \brief To display the update frequency
 * \param freq: The frequency to display
 */
VISHNU_API_LIB void
displayFreq(int freq);

/**
* \brief To display a system information
* \param sysInfo: The system information to display
*/
VISHNU_API_LIB void
displaySystemInfo(IMS_Data::SystemInfo* sysInfo);

/**
 * \brief To display the list of system information
 * \param li: The list of system information to display
 */
VISHNU_API_LIB void
displayListSysInfo(IMS_Data::ListSysInfo* li);

/**
 * \brief  function to convert the type of metric into string
 * \param metricType: The type of metric
 * \return The converted state value
 */
VISHNU_API_LIB std::string
convertMetricToString(const int& metricType);

/**
* \brief To display a system threshold
* \param systemThreshold: The system threshold to display
*/
VISHNU_API_LIB void
displaySystemThreshold(IMS_Data::Threshold* systemThreshold);

/**
 * \brief To display the list of system threshold
 * \param li: The list of system threshold to display
 */
VISHNU_API_LIB void
displayListSysThreshold(IMS_Data::ListThreshold* li);
