#include "api_ims.hpp"

#include <string>

#include "SessionProxy.hpp"
#include "QueryProxy.hpp"
#include "MetricProxy.hpp"
#include "SysInfoProxy.hpp"
#include "ThresholdProxy.hpp"
#include "ProcessCtlProxy.hpp"
#include "ExporterProxy.hpp"
#include <boost/filesystem.hpp>

#include "utilIMS.hpp"
#include "utilVishnu.hpp"
#include "IMSServices.hpp"

int
vishnu::exportCommands(const std::string& sessionKey,
                       const std::string& oldSessionId,
                       const std::string& filename,
                       const IMS_Data::ExportOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  boost::filesystem::path file (filename);
  // Check the existence of file
  if (((false== boost::filesystem::exists(file)) ||
       (true== boost::filesystem::is_directory(file))) ||
      (false== boost::filesystem::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "The file: " + filename +" is a directory or does not exists");
  }

  SessionProxy sessionProxy(sessionKey);
  ExporterProxy exporterProxy(sessionProxy);
  exporterProxy.exportCmd(oldSessionId, filename, op);

  return IMS_SUCCESS;
}

int
vishnu::getMetricCurrentValue(const std::string& sessionKey,
                              const std::string& machineId,
                              IMS_Data::ListMetric& list,
                              const IMS_Data::CurMetricOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  std::string name = std::string(SERVICES_IMS[INT_GETMETRICCURENTVALUE]) + "@" + machineId;
  QueryProxy<IMS_Data::CurMetricOp, IMS_Data::ListMetric>
    query(op, sessionProxy, name, machineId);

  IMS_Data::ListMetric_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory =
      IMS_Data::IMS_DataFactory::_instance();
    for (unsigned int i = 0; i < li->getMetric().size(); i++) {
      IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
      //To copy the content and not the pointer
      *met = *li->getMetric().get(i);
      list.getMetric().push_back(met);
    }
    delete li;
  }
  return IMS_SUCCESS;
}

int
vishnu::getMetricHistory(const std::string& sessionKey,
                         const std::string& machineId,
                         IMS_Data::ListMetric& list,
                         const IMS_Data::MetricHistOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  std::string name = SERVICES_IMS[INT_GETMETRICHISTORY];
  //To check the metric value
  checkMetricHistoryValue(op.getType());

  QueryProxy<IMS_Data::MetricHistOp, IMS_Data::ListMetric>
    query(op, sessionProxy, name, machineId);

  IMS_Data::ListMetric_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory =
      IMS_Data::IMS_DataFactory::_instance();
    for (unsigned int i = 0; i < li->getMetric().size(); i++) {
      IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
      //To copy the content and not the pointer
      *met = *li->getMetric().get(i);
      list.getMetric().push_back(met);
    }
    delete li;
  }
  return IMS_SUCCESS;
}


int
vishnu::getProcesses(const std::string& sessionKey,
                     IMS_Data::ListProcesses& list,
                     const IMS_Data::ProcessOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  std::string name = SERVICES_IMS[INT_GETPROCESSES];
  QueryProxy<IMS_Data::ProcessOp, IMS_Data::ListProcesses>
    query(op, sessionProxy, name);

  IMS_Data::ListProcesses_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory =
      IMS_Data::IMS_DataFactory::_instance();
    for (unsigned int i = 0; i < li->getProcess().size(); i++) {
      IMS_Data::Process_ptr proc = ecoreFactory->createProcess();
      *proc = *li->getProcess().get(i);
      list.getProcess().push_back(proc);
    }
    delete li;
  }

  return IMS_SUCCESS;
}


int
vishnu::setSystemInfo(const std::string& sessionKey, IMS_Data::SystemInfo sys)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  SysInfoProxy sysInfo(sessionProxy);
  sysInfo.setSystemInfo(sys);

  return IMS_SUCCESS;
}


int
vishnu::setSystemThreshold(const std::string& sessionKey,
                           IMS_Data::Threshold threshold)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  //To check the metric value
  checkMetricHistoryValue(threshold.getType());

  SessionProxy sessionProxy(sessionKey);
  ThresholdProxy thresholdProxy(sessionProxy);
  thresholdProxy.setSystemThreshold(threshold);

  return IMS_SUCCESS;
}


int
vishnu::getSystemThreshold(const std::string& sessionKey,
                           IMS_Data::ListThreshold& list,
                           const IMS_Data::ThresholdOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  //To check the metric value
  checkMetricHistoryValue(op.getMetricType());

  SessionProxy sessionProxy(sessionKey);
  ThresholdProxy thresholdProxy(sessionProxy);
  thresholdProxy.getSystemThreshold(list, op);

  return IMS_SUCCESS;
}




int
vishnu::loadShed(const std::string& sessionKey,
                 const std::string& machineId,
                 IMS_Data::LoadShedType loadShedType)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy, machineId);
  processCtlProxy.loadShed(loadShedType);

  return IMS_SUCCESS;
}


int
vishnu::setUpdateFrequency(const std::string& sessionKey, int freq)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {
  SessionProxy sessionProxy(sessionKey);

  MetricProxy met(sessionProxy);
  met.setUpFreq(freq);
  return IMS_SUCCESS;
}


int
vishnu::getUpdateFrequency(const std::string& sessionKey, int freq)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {
  SessionProxy sessionProxy(sessionKey);

  MetricProxy met(sessionProxy);
  freq = met.getUpFreq();
  return IMS_SUCCESS;
}


int
vishnu::stop(const std::string& sessionKey,
             const std::string& machineId,
             const IMS_Data::SupervisorOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy, machineId);
  processCtlProxy.stop(op);

  return IMS_SUCCESS;
}


int
vishnu::getSystemInfo(const std::string& sessionKey,
                      IMS_Data::ListSysInfo& list,
                      const IMS_Data::SysInfoOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  SysInfoProxy sysInfo(sessionProxy);
  sysInfo.getSystemInfo(list, op);

  return IMS_SUCCESS;
}


int
vishnu::restart(const std::string& sessionKey,
                const std::string& machineId,
                const IMS_Data::SupervisorOp& op)
  throw (UMSVishnuException, IMSVishnuException,
         UserException, SystemException) {

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy, machineId);
  processCtlProxy.restart(op);

  return IMS_SUCCESS;
}
