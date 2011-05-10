#include "api_ims.hpp"
#include "SessionProxy.hpp"
#include "QueryProxy.hpp"
#include "MetricProxy.hpp"

int
vishnu::exportCommands(const string sessionKey,
	       const string oldSessionId,
	       string filename,
	       IMS_Data::ExportOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
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
    IMS_Data::Metric_ptr met;
    for(unsigned int i = 0; i < li->getMetric().size(); i++) {
      met = li->getMetric().get(i);
      list.getMetric().push_back(met);
    }
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
  QueryProxy<IMS_Data::MetricHistOp, IMS_Data::ListMetric> 
    query(op, sessionProxy, name, machineId);

  IMS_Data::ListMetric_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::Metric_ptr met;
    for(unsigned int i = 0; i < li->getMetric().size(); i++) {
      met = li->getMetric().get(i);
      list.getMetric().push_back(met);
    }
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
    IMS_Data::Process_ptr proc;
    for(unsigned int i = 0; i < li->getProcess().size(); i++) {
      proc = li->getProcess().get(i);
      list.getProcess().push_back(proc);
    }
  }

  return IMS_SUCCESS;
}



int
vishnu::setSystemInfo(const string sessionKey,
	      IMS_Data::SystemInfo sys)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int 
vishnu::setSystemThreshold(const int sessionKey,
		   IMS_Data::Threshold threshold)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int 
vishnu::getSystemThreshold(const int sessionKey,
		   IMS_Data::ListThreshold& list,
		   IMS_Data::ThresholdOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::defineUserIdentifier(const int sessionKey,
		     string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::defineMachineIdentifier(const int sessionKey,
			string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::defineJobIdentifier(const int sessionKey,
		    string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::defineTransferIdentifier(const int sessionKey,
			 string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::loadShed(const string sessionKey,
	 string machineId,
	 IMS_Data::LoadShedType)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::setUpdateFrequency(const string sessionKey,
		   int freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);  
  string name = "int_setUpdateFrequency";

  MetricProxy met(sessionProxy);
  met.setUpFreq(freq);
  return IMS_SUCCESS;
}


int
vishnu::getUpdateFrequency(const string sessionKey,
		   int& freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  SessionProxy sessionProxy(sessionKey);  
  string name = "int_getUpdateFrequency";

  MetricProxy met(sessionProxy);
  freq = met.getUpFreq();
  return IMS_SUCCESS;
}


int
vishnu::stop(const string sessionKey,
     IMS_Data::Process proc)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::getSystemInfo(const string sessionKey,
	      IMS_Data::ListSysInfo& list,
	      IMS_Data::SysInfoOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
vishnu::restart(const string sessionKey,
	string machineId,
	IMS_Data::RestartOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}

