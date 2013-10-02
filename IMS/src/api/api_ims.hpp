/**
 * \file api_ims.hpp
 * \brief This file contains the VISHNU api functions for IMS package.
 * \author Kevin Coulonb (kevin.coulomb@sysfera.com)
 * \date April 2011
 */

#ifndef __APIIMS__HH_
#define __APIIMS__HH_

#include <iostream>
#include <string>

//UMS include
#include "UserException.hpp"
#include "SystemException.hpp"
#include "UMSVishnuException.hpp"

//IMS include
#include "IMS_Data.hpp"
#include "IMSVishnuException.hpp"
// Include added because of swig generation of python, otherwise should not be used
#include "api_ums.hpp"


namespace vishnu {
/**
 * \brief To export the commands made during a session in a format
 * \param sessionKey: The session key gotten after connection
 * \param oldSessionId: The session id of the session the user wants to export the commands. This session must be closed.
 * \param filename: The file to export the data
 * \param op: The options for the export, currently the export type as shell format is the only possibility
 * \return int : an error code
 */
int
exportCommands(const std::string& sessionKey,
               const std::string& oldSessionId,
               const std::string& filename,
               const IMS_Data::ExportOp& op=IMS_Data::ExportOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To get the current value of a metric
 * \param sessionKey: The session key gotten after connection
 * \param machineId: The id of the machine to get the value on
 * \param val: The metric (OUT)
 * \param op: The options (the type of the metric only)
 * \return int : an error code
 */
int
getMetricCurrentValue(const std::string& sessionKey,
                      const std::string& machineId,
                      IMS_Data::ListMetric& val,
                      const IMS_Data::CurMetricOp& op=IMS_Data::CurMetricOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To get the values of the metrics on a machine
 * \param sessionKey: The session key gotten after connection
 * \param machineId: The id of the machine
 * \param list: OUT, the set of metrics
 * \param op: The options to get the metrics
 * \return int : an error code
 */
int
getMetricHistory(const std::string& sessionKey,
                 const std::string& machineId,
                 IMS_Data::ListMetric& list,
                 const IMS_Data::MetricHistOp& op=IMS_Data::MetricHistOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To get the running vishnu processes
 * \param sessionKey: The session key gotten after connection
 * \param list: The list of the running vishnu processes
 * \param op: Options to get the processes
 * \return int : an error code
 */
int
getProcesses(const std::string& sessionKey,
             IMS_Data::ListProcesses& list,
             const IMS_Data::ProcessOp& op=IMS_Data::ProcessOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To set system information about a machine
 * \param sessionKey: The session key gotten after connection
 * \param sys: The system info to set
 * \return int : an error code
 */
int
setSystemInfo(const std::string& sessionKey,
              IMS_Data::SystemInfo sys)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);
/**
 * \brief To set a threshold
 * \param sessionKey: The session key gotten after connection
 * \param threshold: The threshold to set
 * \return int : an error code
 */
int
setSystemThreshold(const std::string& sessionKey,
                   IMS_Data::Threshold threshold)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To get a set of thresholds on the system
 * \param sessionKey: The session key gotten after connection
 * \param list: The set of threshold
 * \param op: The options to get the thresholds
 * \return int : an error code
 */
int
getSystemThreshold(const std::string& sessionKey,
                   IMS_Data::ListThreshold& list,
                   const IMS_Data::ThresholdOp& op=IMS_Data::ThresholdOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);


/**
 * \brief To load shed a machine
 * \param sessionKey: The session key gotten after connection
 * \param machineId: The id of the machine to load shed
 * \param loadShedType: The type of load shedding, HARD or SOFT. HARD stops all whereas SOFT stops jobs but they can be restarted later
 * \return int : an error code
 */
int
loadShed(const std::string& sessionKey,
         const std::string& machineId,
         IMS_Data::LoadShedType loadShedType)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To change the automatic update frequency of the machines' state
 * \param sessionKey: The session key gotten after connection
 * \param freq: The new frequency to use for the history maker to record the machines' states
 * \return int : an error code
 */
int
setUpdateFrequency(const std::string& sessionKey, int freq)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);
/**
 * \brief To get the automatic update frequency of the machines' state
 * \param sessionKey: The session key gotten after connection
 * \param freq: The frequency used for the history maker to record the machines' states (OUT)
 * \return int : an error code
 */
int
getUpdateFrequency(const std::string& sessionKey, int freq)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To stop a vishnu process
 * \param sessionKey The session key gotten after connection
 * \param machineId The vishnu machine
 * \param op the option
 * \return 0 on success or throws an exception
 */
int
stop(const std::string& sessionKey,
     const std::string& machineId,
     const IMS_Data::SupervisorOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To get system information about a machine
 * \param sessionKey: The session key gotten after connection
 * \param list: The list of the system information
 * \param op: The options, the id of the machine to get the system information about
 * \return int : an error code
 */
int
getSystemInfo(const std::string& sessionKey,
              IMS_Data::ListSysInfo& list,
              const IMS_Data::SysInfoOp& op=IMS_Data::SysInfoOp())
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

/**
 * \brief To restart a vishnu process
 * \param sessionKey: The session key gotten after connection
 * \param machineId: The machine where to restart
 * \param op: The options for the restart, the vishnuConfigurationFile and the type of sed to be restarted on the machine
 * \return int : an error code
 */
int
restart(const std::string& sessionKey,
        const std::string& machineId,
        const IMS_Data::SupervisorOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException);

}

#endif
