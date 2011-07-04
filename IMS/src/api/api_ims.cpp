#include "api_ims.hpp"
#include "SessionProxy.hpp"
#include "QueryProxy.hpp"
#include "MetricProxy.hpp"
#include "SysInfoProxy.hpp"
#include "ObjectIdProxy.hpp"
#include "ThresholdProxy.hpp"
#include "ProcessCtlProxy.hpp"
#include "ExporterProxy.hpp"
#include <boost/filesystem.hpp>

int
vishnu::exportCommands(const string sessionKey,
	       const string oldSessionId,
	       string filename,
	       IMS_Data::ExportOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  boost::filesystem::path file (filename);
  // Check the existence of file
  if (((false== boost::filesystem::exists(file)) || (true== boost::filesystem::is_directory(file)))
    || (false== boost::filesystem::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "The file: " + filename +" is a directory or does not exists");
  }

  SessionProxy sessionProxy(sessionKey);
  ExporterProxy exporterProxy(sessionProxy);
  exporterProxy.exportCmd(oldSessionId, filename, op);

  return IMS_SUCCESS;
}

int
vishnu::getMetricCurrentValue(const string sessionKey,
			      string machineId,
			      IMS_Data::ListMetric& list,
			      IMS_Data::CurMetricOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  string name = "int_getMetricCurentValue"+machineId;
  QueryProxy<IMS_Data::CurMetricOp, IMS_Data::ListMetric>
    query(op, sessionProxy, name, machineId);

  IMS_Data::ListMetric_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
    for(unsigned int i = 0; i < li->getMetric().size(); i++) {
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
vishnu::getMetricHistory(const string sessionKey,
		 string machineId,
		 IMS_Data::ListMetric& list,
		 IMS_Data::MetricHistOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  string name = "int_getMetricHistory";
  //To check the metric value
  checkMetricHistoryValue(op.getType());

  QueryProxy<IMS_Data::MetricHistOp, IMS_Data::ListMetric>
    query(op, sessionProxy, name, machineId);

  IMS_Data::ListMetric_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
    for(unsigned int i = 0; i < li->getMetric().size(); i++) {
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
vishnu::getProcesses(const string sessionKey,
	     IMS_Data::ListProcesses& list,
	     IMS_Data::ProcessOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  string name = "int_getProcesses";
  QueryProxy<IMS_Data::ProcessOp, IMS_Data::ListProcesses>
    query(op, sessionProxy, name);

  IMS_Data::ListProcesses_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
    for(unsigned int i = 0; i < li->getProcess().size(); i++) {
      IMS_Data::Process_ptr proc = ecoreFactory->createProcess();
      *proc = *li->getProcess().get(i);
      list.getProcess().push_back(proc);
    }
    delete li;
  }

  return IMS_SUCCESS;
}



int
vishnu::setSystemInfo(const string sessionKey,
	      IMS_Data::SystemInfo sys)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  SysInfoProxy sysInfo(sessionProxy);
  sysInfo.setSystemInfo(sys);

  return IMS_SUCCESS;
}


int
vishnu::setSystemThreshold(const string sessionKey,
		   IMS_Data::Threshold threshold)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  //To check the metric value
  checkMetricHistoryValue(threshold.getType());

  SessionProxy sessionProxy(sessionKey);
  ThresholdProxy thresholdProxy(sessionProxy);
  thresholdProxy.setSystemThreshold(threshold);

  return IMS_SUCCESS;
}


int
vishnu::getSystemThreshold(const string sessionKey,
		   IMS_Data::ListThreshold& list,
		   IMS_Data::ThresholdOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  //To check the metric value
  checkMetricHistoryValue(op.getMetricType());

  SessionProxy sessionProxy(sessionKey);
  ThresholdProxy thresholdProxy(sessionProxy);
  thresholdProxy.getSystemThreshold(list, op);

  return IMS_SUCCESS;
}


int
vishnu::defineUserIdentifier(const string sessionKey,
		     string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);

  ObjectIdProxy ob(sessionProxy);
  ob.setUID(fmt);
  return IMS_SUCCESS;
}


int
vishnu::defineMachineIdentifier(const string sessionKey,
			string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);

  ObjectIdProxy ob(sessionProxy);
  ob.setMID(fmt);
  return IMS_SUCCESS;
}


int
vishnu::defineJobIdentifier(const string sessionKey,
		    string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);

  ObjectIdProxy ob(sessionProxy);
  ob.setTID(fmt);
  return IMS_SUCCESS;
}


int
vishnu::defineTransferIdentifier(const string sessionKey,
			 string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);

  ObjectIdProxy ob(sessionProxy);
  ob.setFID(fmt);
  return IMS_SUCCESS;
}


int
vishnu::loadShed(const string sessionKey,
	 string machineId,
	 IMS_Data::LoadShedType loadShedType)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy, machineId);
  processCtlProxy.loadShed(loadShedType);

  return IMS_SUCCESS;
}


int
vishnu::setUpdateFrequency(const string sessionKey,
		   int freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);  

  MetricProxy met(sessionProxy);
  met.setUpFreq(freq);
  return IMS_SUCCESS;
}


int
vishnu::getUpdateFrequency(const string sessionKey,
		   int& freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);  

  MetricProxy met(sessionProxy);
  freq = met.getUpFreq();
  return IMS_SUCCESS;
}


int
vishnu::stop(const string sessionKey,
     IMS_Data::Process proc)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy);
  processCtlProxy.stop(proc);

  return IMS_SUCCESS;
}


int
vishnu::getSystemInfo(const string sessionKey,
	      IMS_Data::ListSysInfo& list,
	      IMS_Data::SysInfoOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  SysInfoProxy sysInfo(sessionProxy);
  sysInfo.getSystemInfo(list, op);

  return IMS_SUCCESS;
}


int
vishnu::restart(const string sessionKey,
	string machineId,
	IMS_Data::RestartOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){

  SessionProxy sessionProxy(sessionKey);
  ProcessCtlProxy processCtlProxy(sessionProxy, machineId);
  processCtlProxy.restart(op);

  return IMS_SUCCESS;
}

